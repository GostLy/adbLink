#ifndef SETPDIALOG_H
#define SETPDIALOG_H

#include <QDialog>

namespace Ui {
class setpDialog;
}

class setpDialog : public QDialog
{
    Q_OBJECT

public:

    void setpname(QString pname);
    QString getpname();
    int getbutton();

    explicit setpDialog(QWidget *parent = nullptr);
    ~setpDialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::setpDialog *ui;
};

#endif // SETPDIALOG_H
