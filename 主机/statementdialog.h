#ifndef STATEMENTDIALOG_H
#define STATEMENTDIALOG_H

#include <QDialog>
#include "format.h"

namespace Ui {
class statementDialog;
}

class statementDialog : public QDialog
{
    Q_OBJECT

public:
    explicit statementDialog(QVector<record> bill,QWidget *parent = 0);
    ~statementDialog();

private:
    Ui::statementDialog *ui;

    void closeEvent();
};

#endif // STATEMENTDIALOG_H
