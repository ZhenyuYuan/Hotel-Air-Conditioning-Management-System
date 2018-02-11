#include "logindialog.h"
#include "ui_logindialog.h"
#include <QPushButton>

loginDialog::loginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loginDialog)
{
    Qt::WindowFlags flags = Qt::Dialog;
    flags |= Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);
    ui->setupUi(this);
    ui->lineEdit->setFocus();
    ui->label->setStyleSheet("font-size:16px");
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("确定");
}

loginDialog::~loginDialog()
{
    delete ui;
}

void loginDialog::closeEvent(QCloseEvent *event)
{
    exit(0);
}

void loginDialog::test(QString ID)
{
    qDebug() << ID;
}

void loginDialog::accept()
{
    if (ui->lineEdit->text() != "")
    {
        this->hide();
        send(ui->lineEdit->text());

    }
}

void loginDialog::reject()
{

}
