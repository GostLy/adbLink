#include "uninstalldialog.h"
#include "ui_uninstalldialog.h"
#include <QProcess>
#include <QString>
#include <QMessageBox>
#include <QStringList>
#include <QTextStream>
#include <QDebug>

#ifdef Q_OS_LINUX
 int ost=0;
#elif defined(Q_OS_WIN)
  int ost=1;
#elif defined(Q_OS_MAC)
int ost=2;
#endif

QString commstr;
QString cstr;
QString argument;
QString adb2;
QProcess packages;

QString uninstallDialog::packageName() {
   if (ui->unlistWidget->selectedItems().count() == 1 )
        return ui->unlistWidget->currentItem()->text();
     else return "";
}

bool uninstallDialog::keepBox() {
   return ui->keepBox->isChecked();
}

uninstallDialog::uninstallDialog(const QString &daddr, const QString &port, QWidget *parent) :
   QDialog(parent), m_daddr(daddr), m_port(port),
   ui(new Ui::uninstallDialog) {
   ui->setupUi(this);
   setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

   adb2 = QCoreApplication::applicationDirPath() + "/adbfiles/" + "adb";
   adb2 = '"' + adb2 + '"';

   ui->setupUi(this);
   this->setWindowTitle(daddr);

   QObject::connect(ui->applyButton, SIGNAL(clicked()), this, SLOT(on_applyButton_clicked()));
   QObject::connect(ui->apkclearButton, SIGNAL(clicked()), this, SLOT(on_apkclearButton_clicked()));

   loadList();
   makeFile();
   loadBox();
}

uninstallDialog::~uninstallDialog() {
    delete ui;
}

void uninstallDialog::on_applyButton_clicked() {
    ui->unlistWidget->clear();

    if (ui->lineEdit->text() != "") {
        if (m_port.isEmpty())
            argument = " -s " + m_daddr + " shell pm list packages | grep " + ui->lineEdit->text();
        else
            argument = " -s " + m_daddr + ":" + m_port + " shell pm list packages | grep " + ui->lineEdit->text();

        cstr = adb2 + argument;
    } else {
        loadList();
    }

    makeFile();
    loadBox();
}

void uninstallDialog::loadList() {
    if (m_port.isEmpty())
        argument = " -s " + m_daddr + " shell pm list packages";
    else
        argument = " -s " + m_daddr + ":" + m_port + " shell pm list packages";

    cstr = adb2 + argument;
}

void uninstallDialog::makeFile() {
    packages.start(cstr);
    packages.waitForFinished(-1);
    commstr = packages.readAll();
}

void uninstallDialog::loadBox() {
    QStringList packageList = commstr.split('\n', QString::SkipEmptyParts);

    foreach (QString package, packageList) {
        if (!package.isEmpty()) {
            package.remove(0, 8);
            ui->unlistWidget->addItem(package);
        }
    }
}

void uninstallDialog::on_apkclearButton_clicked() {
    ui->lineEdit->setText("");
}

