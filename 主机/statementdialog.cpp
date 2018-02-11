#include "statementdialog.h"
#include "ui_statementdialog.h"
#include "statementItem.h"
#include "ui_statemenItem.h"
#include <QDebug>
#include "format.h"

statementDialog::statementDialog(QVector<record> bill,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::statementDialog)
{
    ui->setupUi(this);
    statementItem *itemUi;
    QListWidgetItem *nitem;
    QWidget *a;
    record billItem;
    qDebug()<<bill.size()<<"dshhxxxqqqq";
    for (int i =0; i < bill.count(); ++i)
    {
        billItem = bill[i];
        itemUi = new statementItem(billItem);
        nitem = new QListWidgetItem(ui->listWidget);
        nitem->setSizeHint(QSize(0,40));
        a = new QWidget(this);
        //itemUi->setupUi(a);
        ui->listWidget->setItemWidget(nitem,itemUi);
    }

    setAttribute(Qt::WA_DeleteOnClose);
}

statementDialog::~statementDialog()
{
    qDebug()<<"jjj";
    delete ui;
}
