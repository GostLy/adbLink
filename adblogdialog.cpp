#include "adblogdialog.h"
#include "ui_adblogdialog.h"

#include <QMessageBox>

#include <QDir>
#include <QFile>
#include <QTextStream>
#include<QClipboard>
#include <QDebug>



#ifdef Q_OS_WIN
QString logdir = QDir::homePath() + "/AppData/Roaming/.jocala/";
#elif defined(Q_OS_UNIX)
QString logdir = QDir::homePath() + "/.jocala/";
#endif

QString logfile1 = "adblink.log";
QString logfile2 = "adblink.old.log";
QString content;
bool getfile=true;



QClipboard *clipboard = QApplication::clipboard();

void getlog()

{

    QString fn = "";

    if (getfile)
       fn = logfile1;
    else
        fn=logfile2;

    QFile file(logdir+fn);

    if(!file.exists())
       {
       QMessageBox::critical(0, "","Can't find "+fn+"\n",QMessageBox::Cancel);
       getfile=true;
       return;
        }

    file.open(QIODevice::ReadOnly);
    QTextStream stream(&file);
    content = stream.readAll();
    file.close();


}



adblogDialog::adblogDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::adblogDialog)
{
    ui->setupUi(this);

     setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);


    getlog();
    ui->logfileName->setText("adblink.log");
    ui->logBrowser->setPlainText(content);


}





adblogDialog::~adblogDialog()
{
    delete ui;

}

void adblogDialog::on_copyButton_clicked()
{
 clipboard->setText(content);
}

void adblogDialog::on_swapButton_clicked()
{
getfile = !getfile;

if (getfile)
    ui->logfileName->setText("adblink.log");
else
 ui->logfileName->setText("adblink.old.log");

getlog();
ui->logBrowser->setPlainText(content);

}
