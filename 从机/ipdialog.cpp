#include "ipdialog.h"
#include "ui_ipdialog.h"
#include <QPushButton>
#include <QMessageBox>

IPDialog::IPDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IPDialog)
{
    Qt::WindowFlags flags = Qt::Dialog;
    flags |= Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);
    ui->setupUi(this);
    ui->lineEdit->setFocus();
    ui->label->setStyleSheet("font-size:16px");
    ui->label_2->setStyleSheet("font-size:16px");
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("确定");
    ui->lineEdit->setValidator(new QIntValidator(0, 65535, this));
}

IPDialog::~IPDialog()
{
    delete ui;
}

void IPDialog::closeEvent(QCloseEvent *event)
{
    exit(0);
}

void IPDialog::accept()
{
    QString pattern("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    QRegExp rx(pattern);

    if (ui->lineEdit->text() != "" && rx.exactMatch(ui->lineEdit_2->text()))
    {
        this->done(0);
    }
    else
    {
        QMessageBox::information(this,"提示","端口号或ip输入有误");
    }
}

QString IPDialog::getSeverHost()
{
    return ui->lineEdit_2->text();
}

int IPDialog::getPort()
{
    return ui->lineEdit->text().toInt();
}
