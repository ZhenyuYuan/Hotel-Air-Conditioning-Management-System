#include "reportdialog.h"
#include "ui_reportdialog.h"
#include "slavelist.h"
#include <QButtonGroup>
#include <QVector>
#include <QSqlQuery>
#include <QVariant>
#include <QDateTime>
#include <fstream>

const QString DATETIMEFORMAT2="yy-MM-dd";

reportDialog::reportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::reportDialog)
{
    ui->setupUi(this);
    QButtonGroup * group = new QButtonGroup();
    group->addButton(ui->dayRadioButton);
    group->addButton(ui->weekRadioButton);
    group->addButton(ui->monthRadioButton);
    ui->dayRadioButton->setChecked(true);

    ui->dateTimeEdit->setMaximumDateTime(QDateTime::currentDateTime());
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());

    model = new QSqlTableModel;
    model->setTable("bill");

    model->setHeaderData(0,Qt::Horizontal,"顾客号");
    model->setHeaderData(1,Qt::Horizontal,"房间号");
    model->setHeaderData(2,Qt::Horizontal,"起始时间");
    model->setHeaderData(3,Qt::Horizontal,"终止时间");
    model->setHeaderData(4,Qt::Horizontal,"风量");
    model->setHeaderData(5,Qt::Horizontal,"起始温度");
    model->setHeaderData(6,Qt::Horizontal,"终止温度");
    model->setHeaderData(7,Qt::Horizontal,"事件");
    model->setHeaderData(8,Qt::Horizontal,"费用");
    ui->tableView->setModel(model);
    ui->tableView->setColumnWidth(0,50);
    ui->tableView->setColumnWidth(1,50);
    ui->tableView->setColumnWidth(2,160);
    ui->tableView->setColumnWidth(3,160);
    ui->tableView->setColumnWidth(4,50);
    ui->tableView->setColumnWidth(5,50);
    ui->tableView->setColumnWidth(6,50);
    ui->tableView->setColumnWidth(7,50);
    ui->tableView->setColumnWidth(8,50);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QHeaderView * header = ui->tableView->horizontalHeader();
    header->setStretchLastSection(true);
    ui->tableView->show();

    ui->roomNumComboBox->addItem("311c");
    ui->roomNumComboBox->addItem("312c");
    ui->roomNumComboBox->addItem("313c");
    ui->roomNumComboBox->addItem("315c");
}

reportDialog::~reportDialog()
{
    delete ui;
}

void reportDialog::on_buttonOk_clicked()
{
    QDateTime dateTime = ui->dateTimeEdit->dateTime();
    QDateTime maxTime;
    QDateTime minTime;
    if(ui->dayRadioButton->isChecked())   //日报表
    {
        minTime = QDateTime(dateTime.date(),QTime(0,0));
        maxTime = minTime.addDays(1);
    }
    else if(ui->weekRadioButton->isChecked())      //周报表
    {
        minTime = QDateTime(dateTime.date(),QTime(0,0));
        maxTime = minTime.addDays(7);
    }
    else if(ui->monthRadioButton->isChecked())  //月报表
    {
        minTime = QDateTime(QDate(dateTime.date().year(),dateTime.date().month(),1),QTime(0,0));
        maxTime = minTime.addMonths(1);
    }
    QString roomNum = ui->roomNumComboBox->currentText();
    qDebug() << maxTime.toString(DATETIMEFORMAT) << "1111111111";
    qDebug() << minTime.toString(DATETIMEFORMAT) << "2222222222";
    model->setFilter("roomNum='"+roomNum+"' and stopTime<'"+maxTime.toString(DATETIMEFORMAT)+"' and startTime>'"+minTime.toString(DATETIMEFORMAT)+"'");

    model->select();
    ui->tableView->setModel(model);
    ui->tableView->show();

    QSqlQuery query;
    //该房间该日总费用
    query.exec("SELECT cost, wind FROM bill WHERE roomNum='"+roomNum+"' and stopTime<'"+maxTime.toString(DATETIMEFORMAT)+"' and startTime>'"+minTime.toString(DATETIMEFORMAT)+"'");
    double cost = 0;
    double lowWindCost = 0;
    double middleWindCost = 0;
    double highWindCost = 0;
    double currentCost = 0;
    double thisCost = 0;
    int wind;
    double lastCost = 0;
    while(query.next())
    {
        if(query.next()){
            currentCost = query.value(0).toDouble();
            if(query.previous()){
                lastCost = query.value(0).toDouble();
                thisCost = currentCost - lastCost;

                qDebug() << "currentcost" << currentCost;
                qDebug() << "lastcost" << lastCost;
                qDebug() << "thiscost" << thisCost;

                wind = query.value(1).toInt();

                cost += thisCost;
                if(wind == 1)
                {
                    lowWindCost += thisCost;
                }
                else if(wind == 2)
                {
                    middleWindCost+=thisCost;
                }
                else if(wind == 3)
                {
                    highWindCost += thisCost;
                }
            }
        }


    }
    qDebug() << "cost" << cost;
    qDebug() << "lowWindcost" << lowWindCost;
    qDebug() << "MIDDLEcost" << middleWindCost;
    qDebug() << "HIGHcost" << highWindCost;

    ui->costLabel->setText(QString::number(cost,'f',2) + "元");
    ui->label_8->setText(QString::number(lowWindCost,'f',2) + "元");
    ui->label_12->setText(QString::number(middleWindCost,'f',2) + "元");
    ui->label_14->setText(QString::number(highWindCost,'f',2) + "元");


    query.exec("SELECT * FROM bill WHERE roomNum='"+roomNum+"' and stopTime<'"+maxTime.toString(DATETIMEFORMAT)+"' and startTime>'"+minTime.toString(DATETIMEFORMAT)+"' and event = 'start'");
    int powerOnNum = 1;
    while(query.next()){
        powerOnNum++;
    }

    query.exec("SELECT * FROM bill WHERE roomNum='"+roomNum+"' and stopTime<'"+maxTime.toString(DATETIMEFORMAT)+"' and startTime>'"+minTime.toString(DATETIMEFORMAT)+"' and event = 'stop'");
    int powerOffNum = 0;
    while(query.next())
    {
        powerOffNum++;
    }

    query.exec("SELECT * FROM bill WHERE roomNum='"+roomNum+"' and stopTime<'"+maxTime.toString(DATETIMEFORMAT)+"' and startTime>'"+minTime.toString(DATETIMEFORMAT)+"' and event = 'dealt'");
    int dealtNum = 0;
    while(query.next())
    {
        dealtNum++;
    }

    qDebug() << "powerOnNum" << powerOnNum;
    qDebug() << "powerOffNum" << powerOffNum;
    qDebug() << "dealtNum" << dealtNum;

    ui->powerLabel->setText(QString::number(powerOnNum)+"/"+QString::number(powerOffNum));
    ui->label_10->setText(QString::number(dealtNum));


    //该房间该日空调工作时间
    query.exec("SELECT startTime, stopTime FROM bill WHERE roomNum='"+roomNum+"' and stopTime<'"+maxTime.toString(DATETIMEFORMAT)+"' and startTime>'"+minTime.toString(DATETIMEFORMAT)+"'");
    double workTime = 0;
    while(query.next())
    {
        QDateTime startTime = QDateTime::fromString(query.value(0).toString(),DATETIMEFORMAT);
        QDateTime stopTime = QDateTime::fromString(query.value(1).toString(),DATETIMEFORMAT);
        workTime += double(startTime.secsTo(stopTime));
    }
    qDebug() << "workTime" << workTime;
    ui->label_16->setText(QString::number(workTime) + "s");

    //该房间该日达到目标温度次数
    query.exec("SELECT * FROM bill WHERE roomNum='"+roomNum+"' and stopTime<'"+maxTime.toString(DATETIMEFORMAT)+"' and startTime>'"+minTime.toString(DATETIMEFORMAT)+"' and event = 'reachTem'");
    int reachTemNum = 0;
    while(query.next())
    {
        reachTemNum ++;
    }
    ui->label_18->setText(QString::number(reachTemNum));
    qDebug() << "reachTemNum" << reachTemNum;

    QFile fp("房间"+roomNum+"的报表"+minTime.toString(DATETIMEFORMAT2)+"~"+maxTime.toString(DATETIMEFORMAT2)+".txt");
    if (fp.open(QFile::WriteOnly | QIODevice::Truncate)) //覆盖写入
    {
        QTextStream out(&fp);
        out<< "roomNum:" << roomNum << "\r\n";
        out<< "cost\t\t" << "lowWindCost\t" << "middleWindCost\t" << "highWindCost\t" << "workTime\t" << "dealtNum\t" << "powerOnTime\t" << "powerOffTime\t" << "reachTemNum" << "\r\n";
        out << cost << "\t\t" << lowWindCost << "\t\t" << middleWindCost << "\t\t"<< highWindCost << "\t\t" << workTime << "s\t\t"<< dealtNum << "\t\t" << powerOnNum << "\t\t" << powerOffNum << "\t\t" << reachTemNum <<"\r\n";
    }

}

