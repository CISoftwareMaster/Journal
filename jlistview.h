#ifndef JLISTVIEW_H
#define JLISTVIEW_H

#include <QObject>
#include <QListView>

class JListView : public QListView
{
    Q_OBJECT
public:
    JListView(QWidget *parent = nullptr);
};

#endif // JLISTVIEW_H
