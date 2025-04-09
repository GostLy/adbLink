#include "restdialog.h"
#include "ui_restdialog.h"
#include <QProcess>
#include <QFile>
#include <QString>
#include <QMessageBox>
#include <QStringList>
#include <QTextStream>

restDialog::restDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::restDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

restDialog::~restDialog()
{
    delete ui;
}

void restDialog::setadb_restore(const QStringList &list)
{
    ui->listDirectories->clear();
    for (int i = 0; i < list.size(); i++) {
        ui->listDirectories->addItem(list[i]);
    }
    ui->listDirectories->item(0)->setSelected(true);
}

QString restDialog::restore_data_root()
{
    if (ui->listDirectories->currentItem() != nullptr) {
        return ui->listDirectories->currentItem()->text();
    } else if (ui->listDirectories->count() > 0) {
        ui->listDirectories->setCurrentRow(0);
        return ui->listDirectories->currentItem()->text();
    } else {
        // Handle the case when no items are available in the list
        QMessageBox::information(this, "No Items", "No items available in the list.");
        return QString(); // Return an empty string or handle it according to your requirements
    }
}

void restDialog::on_listDirectories_doubleClicked()
{
    if (ui->listDirectories->currentItem() != nullptr) {
        restDialog::accept();
    } else if (ui->listDirectories->count() > 0) {
        ui->listDirectories->setCurrentRow(0);
        restDialog::accept();
    } else {
        // Handle the case when no items are available in the list
        QMessageBox::information(this, "No Items", "No items available in the list.");
    }
}

void restDialog::on_okButton_clicked()
{
    on_listDirectories_doubleClicked();
}

