#include "connectdialog.h"
#include "ui_connectdialog.h"

connectDialog::connectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::connectDialog)
{
    this->setWindowFlags(this->windowFlags()|Qt::FramelessWindowHint);
    ui->setupUi(this);
}

connectDialog::~connectDialog()
{
    delete ui;
}

void connectDialog::success()
{
    ui->label->setText("连接成功！！");
}
