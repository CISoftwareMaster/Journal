#include "jentry.h"

JEntry::JEntry(int id, QString title, QString content)
{
    _id = id;
    _title = title;
    _content = content;
}

// getter functions
int JEntry::id() const
{
    return _id;
}

QString JEntry::title() const
{
    return _title;
}

QString JEntry::content() const
{
    return _content;
}

// setter functions
void JEntry::set_title(QString title)
{
    _title = title;
}

void JEntry::set_content(QString content)
{
    _content = content;
}
