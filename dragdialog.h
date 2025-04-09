#ifndef DRAGDIALOG_H
#define DRAGDIALOG_H

#include <QDialog>
#include <QStringListModel>

QT_BEGIN_NAMESPACE
namespace Ui { class dragDialog; }
QT_END_NAMESPACE

class dragDialog : public QDialog
{
    Q_OBJECT

public:
    explicit dragDialog(QWidget *parent = nullptr);
    ~dragDialog();
    QStringList getFilePaths(); // Function to retrieve the file paths

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private slots:


    void cancelButton();

    void delItemButton();

    void okButton();

private:
    Ui::dragDialog *ui;
    QStringListModel *model;
};

#endif // DRAGDIALOG_H
