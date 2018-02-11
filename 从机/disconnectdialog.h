#ifndef DISCONNECTDIALOG_H
#define DISCONNECTDIALOG_H

#include <QDialog>

namespace Ui {
class disconnectDialog;
}

class disconnectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit disconnectDialog(QWidget *parent = 0);
    ~disconnectDialog();

private slots:
    void closeEvent(QCloseEvent *event);

private:
    Ui::disconnectDialog *ui;
};

#endif // DISCONNECTDIALOG_H
