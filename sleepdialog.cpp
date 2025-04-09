#include "sleepdialog.h"
#include "ui_sleepdialog.h"
#include <QDebug>

int andos;

sleepDialog::sleepDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sleepDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("Set timers");

    ui->screenEdit->setInputMask("#9999999999");
    ui->sleepEdit->setInputMask("#9999999999");

    ui->title->adjustSize();
    ui->deviceLabel->adjustSize();

    ui->title->move((450 - ui->title->size().width()) / 2, 10);
    ui->deviceLabel->move((450 - ui->deviceLabel->size().width()) / 2, 30);

     //ui->screenEdit->setFocusPolicy(Qt::NoFocus);
    // ui->sleepEdit->setFocusPolicy(Qt::NoFocus);





}

sleepDialog::~sleepDialog()
{
    delete ui;
}




QString sleepDialog::sleepValue() {
    return ui->sleepEdit->text();
}


QString sleepDialog::screenValue() {
    return ui->screenEdit->text();
}






void sleepDialog::setcurrentsleep(const QString &csleep)
{

    ui->sleepEdit->setText(csleep);


}




void sleepDialog::setcurrentscreen(const QString &cscreen)
{

    ui->screenEdit->setText(cscreen);



    if (cscreen == "30000")
    {  ui->screenOff->setChecked(false);
        ui->screenOn->setChecked(true);
    }
    else
    {
        ui->screenOff->setChecked(true);
        ui->screenOn->setChecked(false);
    }




}






void sleepDialog::setdevicelabel(const QString &description)
{
    ui->deviceLabel->setText(description);
}


void sleepDialog::setandroidlabel(const QString &android)
{
    ui->androidLabel->setText(android);
    andos=ui->androidLabel->text().toInt();

    //qDebug() << andos;

    if (andos >= 11 )
      {

   //     ui->groupBox_2->setEnabled(false);

        if (ui->sleepEdit->text() == "1")
          {
            ui->sleepOff->setChecked(true);
            ui->sleepOn->setChecked(false);
           }
    else
           {
            ui->sleepOff->setChecked(false);
            ui->sleepOn->setChecked(true);
            }
     }


     else


     {


            if (ui->sleepEdit->text() == "0")
            {
            ui->sleepOff->setChecked(true);
            ui->sleepOn->setChecked(false);
            }
            else
            {
            ui->sleepOff->setChecked(false);
            ui->sleepOn->setChecked(true);
            }


    }



}




void sleepDialog::on_sleepOff_clicked()
{

    if ( ui->androidLabel->text().toInt() >= 11)
    {
        ui->sleepEdit->setText("1");
    }

    else
    {

        ui->sleepEdit->setText("0");
    }





}

void sleepDialog::on_sleepOn_clicked()
{

    if ( ui->androidLabel->text().toInt() >= 11)
    {
        ui->sleepEdit->setText("0");
    }

    else
    {

        ui->sleepEdit->setText("1200000");
    }

}




void sleepDialog::on_screenOff_clicked()
{
    // screensaver off  2147460000
    ui->screenEdit->setText("2147460000");
    ui->screenOff->setChecked(true);
    ui->screenOn->setChecked(false);
}

void sleepDialog::on_screenOn_clicked()
{
    // default screensaver 300000
    ui->screenEdit->setText("300000");
    ui->screenOff->setChecked(false);
    ui->screenOn->setChecked(true);

}

