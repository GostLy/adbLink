// deviceinfodialog.cpp
#include "deviceinfodialog.h"
#include "ui_deviceinfodialog.h"
#include <QString>
#include <QStringList>
#include <QDebug>

QString deviceinfoDialog::pad_qstring(const QString& qstring, int length) {
    return qstring.leftJustified(length, ' ', Qt::AlignLeft);
}

deviceinfoDialog::deviceinfoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::deviceinfoDialog)
{
    ui->setupUi(this);
}

deviceinfoDialog::~deviceinfoDialog()
{
    delete ui;
}

void deviceinfoDialog::devinfo(const QStringList &list)
{
    QString archi = list.at(0);
    QString droid = list.at(1);
    bool scoped = (list.at(2).toLower() == "true");
    QString device = list.at(3);
    QString manufact = list.at(4);




    int av = droid.toInt();

    ui->scoped->setFixedHeight(100);

    int xPosition = 20;
    int yPosition = 25;
    int labelWidth = 300;
    int labelHeight = 32;

    ui->archilabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    ui->archilabel->setFixedWidth(labelWidth);
    ui->archilabel->setFixedHeight(labelHeight);
    ui->archilabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    ui->archilabel->move(xPosition, yPosition);
    ui->archilabel->setText("System architecture:   " + archi);

    ui->droidlabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    ui->droidlabel->setFixedWidth(labelWidth);
    ui->droidlabel->setFixedHeight(labelHeight);
    ui->droidlabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    ui->droidlabel->move(xPosition, yPosition + labelHeight);

    if (manufact.contains(QLatin1String("amazon"), Qt::CaseInsensitive))
        ui->droidlabel->setText(QLatin1String("Fire OS version:   ") + droid);
    else
        ui->droidlabel->setText(QLatin1String("Android version:   ") + droid);


    ui->devicelabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    ui->devicelabel->setFixedWidth(labelWidth);
    ui->devicelabel->setFixedHeight(labelHeight);
    ui->devicelabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    ui->devicelabel->move(xPosition, yPosition + 2 * labelHeight);
    ui->devicelabel->setText("Device name:   " + device);

    ui->manufactlabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    ui->manufactlabel->setFixedWidth(labelWidth);
    ui->manufactlabel->setFixedHeight(labelHeight);
    ui->manufactlabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    ui->manufactlabel->move(xPosition, yPosition + 3 * labelHeight);
    ui->manufactlabel->setText("Manufacturer:   " + manufact);

    if (av >= 11) {
        if (!scoped)
            ui->scoped->setText("/sdcard/Android/data/ is not restricted by scoped storage.");
        else
            ui->scoped->setText("Scoped storage is in effect. /sdcard/Android/data is sandboxed and can't be accessed by adbLink.");
    } else {
        ui->scoped->setText("Scoped storage is not implemented on this device.");
    }





    ui->archilabel->move(xPosition, yPosition);

    yPosition += 25;
    ui->droidlabel->move(xPosition, yPosition);

    yPosition += 25;
    ui->devicelabel->move(xPosition, yPosition);

    yPosition += 25;
    ui->manufactlabel->move(xPosition, yPosition);

    yPosition += 15;
    ui->scoped->move(xPosition, yPosition);
}
