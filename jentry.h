#ifndef JENTRY_H
#define JENTRY_H

#include <QObject>

class JEntry
{
public:
    JEntry(int id, QString title="", QString content="");

    void set_title(QString title);
    void set_content(QString content);

    int id() const;
    QString title() const;
    QString content() const;

private:
    int _id;
    QString _title;
    QString _content;

};

#endif // JENTRY_H
