#ifndef DATADIALOG_H
#define DATADIALOG_H

#include <QDialog>

namespace Ui {
class dataDialog;
}

class dataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit dataDialog(QWidget *parent = 0);
    ~dataDialog();

public:
    int returnval2();
    QString externalLocation();
   void setadb_data(const QStringList &list);
 //  void setadb_dir(const QString &adb_dir);



private slots:
    void on_dataButton1_clicked();

    void on_dataButton2_clicked();

    void on_dataButton3_clicked();

    void on_dataButton4_clicked();



    void on_okButton_clicked();

   void on_listDirectories1_doubleClicked();


private:
    Ui::dataDialog *ui;
};

extern int os_data;
extern int rval2;
extern QString adbloc;
extern QString adb_data;
extern QString adbdir_data_data;
extern QString tmpdir_data;
extern QString external_location;


#endif // DATADIALOG_H
