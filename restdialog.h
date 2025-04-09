#ifndef RESTDIALOG_H
#define RESTDIALOG_H

#include <QDialog>
#include <QStringList>

namespace Ui {
class restDialog;
}

class restDialog : public QDialog
{
    Q_OBJECT

public:
    explicit restDialog(QWidget *parent = nullptr);
    ~restDialog();

    void setadb_restore(const QStringList &list);
    QString restore_data_root();

private slots:
    void on_listDirectories_doubleClicked();
    void on_okButton_clicked();

private:
    Ui::restDialog *ui;
};

#endif // RESTDIALOG_H


/*
#ifndef RESTDIALOG_H
#define RESTDIALOG_H

#include <QDialog>

namespace Ui {
class restDialog;
}

class restDialog : public QDialog
{
    Q_OBJECT

public:
    explicit restDialog(QWidget *parent = 0);
    ~restDialog();

 public:
   QString restore_data_root();

   void setadb_restore(const QStringList &list);



private slots:



    QString on_listDirectories_doubleClicked();

    void on_okButton_clicked();

private:
    Ui::restDialog *ui;
};




#endif // RESTDIALOG_H
*/
