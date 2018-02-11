#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QCloseEvent>
#include <QString>
#include "slave.h"

namespace Ui {
class loginDialog;
}

class loginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit loginDialog(QWidget *parent = 0);
    void send(QString ID){emit getCusID(ID);}
    void sendHide(){emit Hide();}
    ~loginDialog();

signals:
    void getCusID(QString ID);
    void Hide();

private slots:
    void test(QString ID);

    void accept();

    void reject();

private:
    Ui::loginDialog *ui;
    void closeEvent(QCloseEvent *event);
};

#endif // LOGINDIALOG_H
