#include "jmainwindow.h"
#include "ui_jmainwindow.h"

JMainWindow::JMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::JMainWindow)
{
    ui->setupUi(this);

    // default value for the selection index
    selection_index = -1;

    // initialise and set up our list view model
    model = new JListViewModel;
    ui->entries->setModel(model);

    // handle connections
    connect(ui->addEntryBtn, SIGNAL(clicked(bool)),
            this, SLOT(new_item()));

    connect(ui->saveEntryBtn, SIGNAL(clicked(bool)),
            this, SLOT(save()));

    connect(ui->removeEntryBtn, SIGNAL(clicked(bool)),
            this, SLOT(delete_item()));

    connect(ui->datePicker, SIGNAL(activated(QDate)),
            this, SLOT(date_changed(QDate)));

    connect(ui->entries, SIGNAL(clicked(QModelIndex)),
            this, SLOT(entry_list_clicked(QModelIndex)));

    connect(model, SIGNAL(refresh_complete(int)),
            this, SLOT(count_updated(int)));

    // initial refresh
    model->refresh(QDate::currentDate());
}

void JMainWindow::new_item()
{
    // insert a new entry at the selected date
    model->new_item(ui->datePicker->selectedDate());
}

void JMainWindow::count_updated(int count)
{
    // update entry counter label
    ui->entryCountLabel->setText(tr("%1 item(s) loaded.").arg(count));
}

void JMainWindow::date_changed(QDate new_date)
{
    // deselect current entry
    deselect();

    // refresh entries
    model->refresh(new_date);
}

void JMainWindow::save()
{
    if (selection_index != -1)
    {
        // get current selection
        JEntry entry = model->entry_at(selection_index);

        // get updated content
        QString title = ui->entryTitle->text();
        QString content = ui->entryContent->toPlainText();

        // perform update
        model->update_item(entry.id(), title, content);

        // refresh
        model->refresh(ui->datePicker->selectedDate());
    }
}

void JMainWindow::deselect()
{
    // reset selection index
    selection_index = -1;

    // delete entry editor content
    ui->entryContent->setText("");
    ui->entryTitle->setText("");

    // disable entry editor elements
    ui->entryContent->setDisabled(true);
    ui->entryTitle->setDisabled(true);
    ui->saveEntryBtn->setDisabled(true);
}

void JMainWindow::entry_select()
{
    if (selection_index != -1)
    {
        JEntry entry = model->entry_at(selection_index);

        // activate entry editor elements
        ui->entryContent->setDisabled(false);
        ui->entryTitle->setDisabled(false);
        ui->saveEntryBtn->setDisabled(false);

        // populate element values
        ui->entryContent->setText(entry.content());
        ui->entryTitle->setText(entry.title());
    }
}

void JMainWindow::delete_item()
{
    if (selection_index != -1)
    {
        // get currently selected entry
        JEntry entry = model->entry_at(selection_index);

        // confirm deletion
        if (QMessageBox::information(this,
                                     tr("Do you really want to delete this?"),
                                     tr("Once you click yes, this note can not be recovered!"),
                                     QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            // maximum extermination!
            model->delete_item(entry.id());

            // refresh
            model->refresh(ui->datePicker->selectedDate());
        }
    }
}

void JMainWindow::entry_list_clicked(QModelIndex index)
{
    if (index.row() >= 0 && index.row() < model->item_count())
    {
        // update selection index
        selection_index = index.row();

        // select entry
        entry_select();
    }
}

JMainWindow::~JMainWindow()
{
    delete ui;
    delete model;
}
