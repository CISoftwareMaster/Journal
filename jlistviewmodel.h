#ifndef JLISTVIEWMODEL_H
#define JLISTVIEWMODEL_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>
#include <QAbstractListModel>
#include <QList>
#include <QDebug>

#include "jentry.h"

class JListViewModel : public QAbstractListModel
{
    Q_OBJECT
public:
    JListViewModel(QObject *parent = nullptr);
    ~JListViewModel();

    int item_count() const;
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    void new_item(QDate target);
    void update_item(int eid, QString title, QString content);
    void delete_item(int eid);

    JEntry entry_at(int index) const;

signals:
    void refresh_complete(int item_count);

public slots:
    void refresh(QDate target);

private:
    QList<JEntry> entries;
    QSqlDatabase database;
};

#endif // JLISTVIEWMODEL_H
