#include "datadialog.h"
#include "ui_datadialog.h"
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QtWidgets>
#include <QMessageBox>


int rval2;
QString external_location;


int dataDialog::returnval2() {
    return rval2;
}

QString dataDialog::externalLocation() {
    return external_location;
}

dataDialog::dataDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dataDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    rval2 = 1;
}

dataDialog::~dataDialog()
{
    delete ui;
}

void dataDialog::setadb_data(const QStringList &list)
{
    ui->listDirectories1->clear();
    for (int i = 0; i < list.size(); i++) {
        ui->listDirectories1->addItem(list[i]);
    }
    ui->listDirectories1->item(0)->setSelected(true);
}

void dataDialog::on_dataButton1_clicked()
{
    rval2 = 1;
}

void dataDialog::on_dataButton2_clicked()
{
    rval2 = 2;
}

void dataDialog::on_dataButton3_clicked()
{
    rval2 = 3;
}

void dataDialog::on_dataButton4_clicked()
{
    rval2 = 4;
}

void dataDialog::on_listDirectories1_doubleClicked()
{
    if (ui->listDirectories1->currentItem() != nullptr) {
        external_location = ui->listDirectories1->currentItem()->text();
    } else if (ui->listDirectories1->count() > 0) {
        ui->listDirectories1->setCurrentRow(0);
        external_location = ui->listDirectories1->currentItem()->text();
    } else {
        // Handle the case when no items are available in the list
        QMessageBox::information(this, "No Items", "No items available in the list.");
        return;
    }

    dataDialog::accept();
}

void dataDialog::on_okButton_clicked()
{
    on_listDirectories1_doubleClicked();
}

