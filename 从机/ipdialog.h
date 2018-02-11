#ifndef IPDIALOG_H
#define IPDIALOG_H

#include <QDialog>
#include <QCloseEvent>
#include "slave.h"

namespace Ui {
class IPDialog;
}

class IPDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IPDialog(QWidget *parent = 0);
    QString getSeverHost();
    int getPort();
    ~IPDialog();

signals:

private slots:

    void accept();

private:
    Ui::IPDialog *ui;
    void closeEvent(QCloseEvent *event);
};

#endif // IPDIALOG_H
