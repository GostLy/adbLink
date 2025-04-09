#include "scpdialog.h"
#include "ui_scpdialog.h"

scpDialog::scpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::scpDialog)
{
    ui->setupUi(this);
}

scpDialog::~scpDialog()
{
    delete ui;
}



QString scpDialog::scpArgs() {
   return ui->scpargs->text();
}




void scpDialog::setArgs(QString myargs)
{
    ui->scpargs->setText(myargs);
}


void scpDialog::on_pushButton_clicked()
{
    ui->scpargs->setText("");
}
