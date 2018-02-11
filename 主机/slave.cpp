#include "slave.h"
#include "format.h"


#include <QDate>
#include <QCoreApplication>
#include <QVector>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtSql>
#include <QString>
#include <QSqlError>
#include <fstream>



Slave::Slave(QString no)
{
       this->no = no;
       this->customerID = "";
       this->currentTemperature = 25;
       this->targetTemperature = 25;
       this->wind = MEDIUM;
       this->isStart = STOP;
       this->isWorking = NODEALING;
       this->stopWind = STOPWIND;
       this->totalCost = 0.0;
       this->workMode = 0;
}

//开关机
//void Slave::initSlaveStatus(){
//    this->isStart = !this->isStart;
//    if(this->isStart == 0)
//        sendslaveoff("818",isStart);
//    else
//        sendslaveon("818",isStart,customerID);
//}

//获得customerID
void Slave::initCustomerId(QString no){
    this->customerID = no;
}

//从界面过来的命令，从机目标温度设置
//void Slave::initSlaveTemperature(const int targetTemperature){
//    this->targetTemperature = targetTemperature;
//    sendTarTemperature("818",targetTemperature);
//}

//从机风速设置
//void Slave::initSlaveWind(const int wind){
//    this->wind = wind;
//    sendwind("818",wind);
//}

//从主机过来的从机当前情况（花费）
//void Slave::initSlaveCost(const double cost){
//    this->cost = cost;
//    sendcost("818",cost);
//}

void Slave::getWorkMode(const int workMode){
    this->workMode = workMode;
}

//void Slave::slaveWorkSet(const int isDealing){
//    this->isWorking = isDealing;
//}


void Slave::setTotalCost(double cost){
    totalCost += cost;
}

/*
void Slave::setRecord(QString customerID, QDateTime startTime, QDateTime stopTime, int wind, int targetTemperature, double finalTemperature, double energy, double cost){
    this->record.customerID = customerID;
    this->record.startTime = startTime;
    this->record.stopTime = stopTime;
    this->record.wind = wind;
    this->record.targetTemperature = targetTemperature;
    this->record.finalTemperature = finalTemperature;
    this->record.energy = energy;
    this->record.cost = cost;
}
*/

QVector<record> Slave::createBill()
{
    QVector<record> bill;    //存详单
    record rec;
    QSqlQuery query;
    query.exec("select * from bill where customerID= '" +customerID+ "'");
    while(query.next())
    {
        rec.customerID = query.value(0).toString();
        rec.roomNum = query.value(1).toString();
        rec.startTime = query.value(2).toString();
        rec.stopTime = query.value(3).toString();
        rec.wind =query.value(4).toInt();
        rec.targetTemperature = query.value(5).toInt();
        rec.finalTemperature = query.value(6).toDouble();
        //rec.energy = query.value(7).toDouble();
        rec.event = query.value(7).toString();
        rec.cost = query.value(8).toDouble();


       // record *rec = new setRecord(customerID, roomNum, startTime, stopTime, wind, targetTemperature, finalTemperature, energy, cost);
        qDebug()<<rec.customerID<<"0oplk";
        bill.append(rec);
    }

    QFile fp("顾客"+this->getCustomerID()+"的消费详单.txt");
    if (fp.open(QFile::WriteOnly | QIODevice::Truncate)) //覆盖写入
    {
        QTextStream out(&fp);
        out<< "customerID:" << this->getCustomerID() << "\r\n";
        out<< "roomNum:" << this->getno() << "\r\n";
        out<< "startTime\t\t\t" << "stopTime\t\t\t\t" << "wind\t" << "targetTemperature\t" << "finalTemerature\t" << "energy\t" << "cost\t" << "\r\n";
        for(int i=0; i<=bill.size()-1; i++)
        {
            out << bill[i].startTime << "\t" << bill[i].stopTime << "/t" << bill[i].wind << "\t"<< bill[i].targetTemperature << "\t" << bill[i].finalTemperature << "\t"<< bill[i].event << "\t" << bill[i].cost << "\r\n";
        }
    }
     qDebug()<<bill.size()<<"qqqqweer";
    return bill;
}


