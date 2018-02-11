#include "statementitem.h"
#include "ui_statemenItem.h"

statementItem::statementItem(record billItem, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::statementItem)
{
    ui->setupUi(this);
    ui->startTimeLabel->setText("开始时间：" + billItem.startTime);
    ui->stopTimeLabel->setText("结束时间：" + billItem.stopTime);
    ui->startTimeLabel_2->setText("目标温度：" + QString::number(billItem.targetTemperature));
    ui->finishTemLabel->setText("结束温度：" + QString::number(billItem.finalTemperature));
    if (billItem.wind == 1)
    {
        ui->windLabel->setText("风速：小风");
    }
    else if (billItem.wind == 2)
    {
        ui->windLabel->setText("风速：中风");
    }
    else
    {
        ui->windLabel->setText("风速：大风");
    }

    ui->eneryLabel->setText("事件：" + billItem.event);
    ui->costLabel->setText("费用：" + QString::number(billItem.cost));
}

statementItem::~statementItem()
{
    delete ui;
}
