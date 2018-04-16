#include "jlistviewmodel.h"

JListViewModel::JListViewModel(QObject *parent)
    : QAbstractListModel(parent)
{
    // establish connection to our database
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName("journal.db");
    database.open();
}

int JListViewModel::item_count() const
{
    return entries.length();
}

int JListViewModel::rowCount(const QModelIndex &) const
{
    return item_count();
}

QVariant JListViewModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        int row = index.row();

        if (row >= 0 && row < entries.length())
        {
            return entries[row].title();
        }
    }

    return QVariant();
}

void JListViewModel::refresh(QDate target)
{
    if (database.transaction())
    {
        // fetch entries from a target date
        QSqlQuery query;
        query.prepare("SELECT * FROM entries WHERE edate=:edate");
        query.bindValue(":edate", target);

        // perform request
        bool success = query.exec();
        database.commit();

        // clear our current entries list
        entries.clear();

        // iterate through our results
        if (success)
        {
            // populate our model with entries
            while (query.next())
            {
                JEntry entry(query.value("eid").toInt());

                entry.set_title(query.value("title").toString());
                entry.set_content(query.value("content").toString());

                entries.append(entry);
            }

            // reflect changes to the user interface
            emit refresh_complete(entries.length());
            emit layoutChanged();
        }
    }
}

void JListViewModel::new_item(QDate target)
{
    if (database.transaction())
    {
        // insert an untitled entry at the target date
        QSqlQuery query;
        query.prepare("INSERT INTO entries (title, content, edate) VALUES ('Untitled', '', :edate)");
        query.bindValue(":edate", target);

        // execute our query
        bool success = query.exec();

        database.commit();

        if (success)
        {
            // update our user interface
            refresh(target);
        }
    }
}

void JListViewModel::update_item(int eid, QString title, QString content)
{
    if (database.transaction())
    {
        // update the target item
        QSqlQuery query;
        query.prepare("UPDATE entries SET title=:title, content=:content WHERE eid=:eid");

        // bind values
        query.bindValue(":eid", eid);
        query.bindValue(":title", title);
        query.bindValue(":content", content);

        // execute query
        query.exec();

        database.commit();
    }
}

void JListViewModel::delete_item(int eid)
{
    if (database.transaction())
    {
        // delete the target item
        QSqlQuery query;
        query.prepare("DELETE FROM entries WHERE eid=:eid");
        query.bindValue(":eid", eid);

        // exterminate!
        query.exec();
        database.commit();
    }
}

JEntry JListViewModel::entry_at(int index) const
{
    return entries.at(index);
}

JListViewModel::~JListViewModel()
{
    // terminate connection to our database
    database.close();
}
