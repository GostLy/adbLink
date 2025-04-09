#ifndef ADBPREFDIALOG_H
#define ADBPREFDIALOG_H

#include <QDialog>

namespace Ui {
class adbprefDialog;
}

class adbprefDialog : public QDialog
{
    Q_OBJECT

public:
    explicit adbprefDialog(QWidget *parent = 0);
    ~adbprefDialog();

    QString version2;

     void setversioncheck(const bool &versioncheck);
      void setscopecheck(const bool &scopecheck);
     void setscrcpyargs(const bool &scrcpyargs);
     void setoldfm(const bool &oldfm);

    void setmacterm(int macterm);
    void setlinterm(int linterm);
    void setwinterm(int winterm);
    void changelog();
    void setversionLabel(const QString &versiontext);

    void setdownloaddir(const QString &ddir);

void setinstalldir(const QString &idir);
void setbackupdir(const QString &bdir);



bool versioncheck();
bool scopecheck();
bool scrcpyargs();
bool oldfm();
QString downloaddir();
QString installdir();
QString backupdir();


QString linterm();
QString macterm();
QString winterm();
bool msgboxtype();


private slots:
    void on_checkButton_clicked();
    void onRequestCompleted();

    void on_downloadButton_clicked();

    QString strip2 (QString str);



    void on_installButton_clicked();

    void on_backupButton_clicked();


private:
    Ui::adbprefDialog *ui;
};

#endif // ADBPREFDIALOG_H
