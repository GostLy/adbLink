#include "klogdialog.h"
#include "ui_klogdialog.h"
#include "getadbdata.h"
#include <QProcess>
#include <QMessageBox>
#include<QClipboard>




#ifdef Q_OS_LINUX
 int kos1=0;
#elif defined(Q_OS_WIN)
  int kos1=1;
#elif defined(Q_OS_MAC)
int kos1=2;
#endif

QClipboard *kclipboard = QApplication::clipboard();

klogDialog::klogDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::klogDialog)
{
    ui->setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);



}

klogDialog::~klogDialog()
{
    delete ui;
}



void klogDialog::passdata(const QString &adbpath, const QString &dataroot, const QString &filepath, const QString xbmcpackage)
{

    kfilepath = filepath;
    kdataroot = dataroot;
    kadbpath  = adbpath;
    kpackage  = xbmcpackage;


    ui->klogfileName->setText(klogfile1);

    QString cstring = adbpath + " shell cat "+ filepath+klogfile1;




QString command=getadbOutput(cstring);

ui->klogBrowser->setPlainText(command);
kcontent = command;

}


void klogDialog::on_kswapButton_clicked()
{


    kgetfile = !kgetfile;

    if (kgetfile)
       ui->klogfileName->setText(klogfile1);
     else
       ui->klogfileName->setText(klogfile2);


    QString cstring = kadbpath + " shell cat "+kfilepath+ui->klogfileName->text();
    QString command=getadbOutput(cstring);


    if (command.contains("No such file or directory"))
       command="";

    ui->klogBrowser->setPlainText(command);
    kcontent = command;

}

void klogDialog::on_kcopyButton_clicked()
{
    kclipboard->setText(kcontent);
    klogDialog::accept();

}
