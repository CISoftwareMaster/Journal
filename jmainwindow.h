#ifndef JMAINWINDOW_H
#define JMAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTextEdit>
#include <QDate>

#include "jlistview.h"
#include "jlistviewmodel.h"

namespace Ui {
class JMainWindow;
}

class JMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit JMainWindow(QWidget *parent = 0);
    ~JMainWindow();

private:
    void deselect();
    void entry_select();

    int selection_index;

    Ui::JMainWindow *ui;
    JListViewModel *model;

private slots:
    void new_item();
    void save();
    void delete_item();
    void count_updated(int);
    void date_changed(QDate);
    void entry_list_clicked(QModelIndex);
};

#endif // JMAINWINDOW_H
