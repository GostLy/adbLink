#ifndef ADBLOGDIALOG_H
#define ADBLOGDIALOG_H

#include <QDialog>
#include <QClipboard>

namespace Ui {
class adblogDialog;
}

class adblogDialog : public QDialog
{
    Q_OBJECT


public:
    explicit adblogDialog(QWidget *parent = 0);
    ~adblogDialog();



private slots:
    void on_copyButton_clicked();

    void on_swapButton_clicked();

private:
    Ui::adblogDialog *ui;
};

extern int os1;
extern QString logdir;
extern QString logfile1;
extern QString logfile2;
extern QString content;
extern bool getfile;

extern QClipboard *clipboard;

#endif // ADBLOGDIALOG_H
