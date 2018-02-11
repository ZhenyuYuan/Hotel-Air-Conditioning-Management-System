#include "disconnectdialog.h"
#include "ui_disconnectdialog.h"

disconnectDialog::disconnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::disconnectDialog)
{
    Qt::WindowFlags flags = Qt::Dialog;
    flags |= Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);
    ui->setupUi(this);
    ui->label->setStyleSheet("font-size:16px");
}

disconnectDialog::~disconnectDialog()
{
    delete ui;
}

void disconnectDialog::closeEvent(QCloseEvent *event)
{
    exit(0);
}
