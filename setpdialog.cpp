#include "setpdialog.h"
#include "ui_setpdialog.h"

setpDialog::setpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setpDialog)
{
    ui->setupUi(this);
}

setpDialog::~setpDialog()
{
    delete ui;
}



void setpDialog::setpname(QString pname)
{
    ui->pname->setText(pname);
}


QString setpDialog::getpname() {
    return ui->pname->text();
}


int setpDialog::getbutton() {
    return ui->allowButton->isChecked();
}

void setpDialog::on_pushButton_clicked()
{
    ui->pname->setText("");
}

