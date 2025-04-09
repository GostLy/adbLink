#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>

namespace Ui {
class Dialog2;
}

class Dialog2 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog2(QWidget *parent = 0);
    ~Dialog2();

    void setaLabel(const QString &atext);

private slots:
    void on_pushButton_clicked();





    void on_donate_clicked();

private:
    Ui::Dialog2 *ui;
};

#endif // ABOUT_H
