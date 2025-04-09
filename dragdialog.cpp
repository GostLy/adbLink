#include "dragdialog.h"
#include "ui_dragdialog.h"
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QListWidgetItem>
#include <QMimeData>
#include <QDebug>
#include <QStringListModel>

dragDialog::dragDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dragDialog)
{
    ui->setupUi(this);

    ui->listView->setAcceptDrops(true);
    ui->listView->setDragEnabled(false); // Enable dragging items within the list

    model = new QStringListModel(this);
    ui->listView->setModel(model);

    connect(ui->pushButton1, &QPushButton::released, this, &dragDialog::cancelButton);
    connect(ui->pushButton2, &QPushButton::released, this, &dragDialog::delItemButton);
    connect(ui->pushButton3, &QPushButton::released, this, &dragDialog::okButton);
}

dragDialog::~dragDialog()
{
    delete ui;
}


void dragDialog::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void dragDialog::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void dragDialog::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void dragDialog::dropEvent(QDropEvent *event)
{
    const QMimeData* mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        QList<QUrl> urlList = mimeData->urls();
        QStringList droppedItems;
        // Get the existing items from the model
        droppedItems = model->stringList();

        for (const QUrl& url : urlList) {
            if (url.isLocalFile()) {
                QString filePath = url.toLocalFile();
                droppedItems.append(filePath);
            }
        }
        model->setStringList(droppedItems); // Update the model to display dropped items in the QListView
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}



void dragDialog::delItemButton()
{
   QModelIndexList selectedIndexes = ui->listView->selectionModel()->selectedIndexes();

   if (selectedIndexes.size() == 1) {
       int row = selectedIndexes.first().row();
       model->removeRow(row);
   }
}



QStringList dragDialog::getFilePaths()
{
    return model->stringList();
}

void dragDialog::cancelButton()
{


    reject();

}

void dragDialog::okButton()
{
    accept();
}
