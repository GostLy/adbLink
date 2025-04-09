#ifndef OSLOGDIALOG_H
#define OSLOGDIALOG_H

#include <QDialog>

namespace Ui {
class oslogDialog;
}

class oslogDialog : public QDialog
{
    Q_OBJECT

public:
    explicit oslogDialog(QWidget *parent = 0);
    ~oslogDialog();

private:


   QString logostype;

public:
    void ospassdata(const QString &logostype);




private slots:
    void on_oscopyButton_clicked();

    void on_osswapButton_clicked();

private:
    Ui::oslogDialog *ui;
};



extern int los1;
extern bool osgetfile;
extern QString oslogfile1;
extern QString oslogfile2;
extern QString currlog;
extern QString oldlog;
extern QString cliplog;
extern QString logText;
extern QString oldlogText;
extern QClipboard *osclipboard;






#endif // OSLOGDIALOG_H
