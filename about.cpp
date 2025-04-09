#include "about.h"
#include "ui_about.h"
#include <QDesktopServices>
#include <QUrl>



void Dialog2::setaLabel(const QString &atext)
{
    ui->alabel->setText(atext);
}

Dialog2::Dialog2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog2)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);


    ui->betaLabel->setVisible(false);


    ui->alabel->setAlignment(Qt::AlignCenter);
    ui->betaLabel->setAlignment(Qt::AlignCenter);
    ui->thankLabel->setAlignment(Qt::AlignCenter);
    ui->linkLabel->setAlignment(Qt::AlignCenter);



    QPixmap pix(":/assets/donate.png");
    QIcon icon(pix);
    ui->donate->setIcon(icon);
    ui->donate->setIconSize(pix.size());




}

Dialog2::~Dialog2()
{
    delete ui;
}

void Dialog2::on_pushButton_clicked()
{

}

void Dialog2::on_donate_clicked()
{
   QString link = "https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=GKZMW456H6E5W";
   QDesktopServices::openUrl(QUrl(link));


}
