// deviceinfodialog.h
#ifndef DEVICEINFODIALOG_H
#define DEVICEINFODIALOG_H

#include <QDialog>

namespace Ui {
class deviceinfoDialog;
}

class deviceinfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit deviceinfoDialog(QWidget *parent = nullptr);
    ~deviceinfoDialog();

    void devinfo(const QStringList &list);

private slots:


    QString pad_qstring(const QString& qstring, int length);


private:
    Ui::deviceinfoDialog *ui;
};

#endif // DEVICEINFODIALOG_H

