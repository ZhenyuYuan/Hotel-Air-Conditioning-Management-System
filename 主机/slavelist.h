#ifndef SLAVELIST_H
#define SLAVELIST_H

#include "slave.h"
#include"format.h"
#include <QVector>
#include <QDialog>
#include <QObject>
#include <server.h>

class slaveList : public QObject{
    Q_OBJECT
private:


    int workingNum;//当前正在工作的从机数量
    //下面存储主机设置
    bool isWorkingMain=true;             //主机是否启动
    int workmodeMain;          //工作模式(制冷、制热)
    double minTemperatureMain;       //最低工作温度
    double maxTemperatureMain;       //最高工作温度
    double defTemperatureMain;       //缺省工作温度
    double costPerWMain;             //单位功率费用
    int defaultWind;                //默认初始化风速


public:
    QVector<Slave*> *slavelist;
    Server *server;
    slaveList();
    //~slaveList();
    //QVector<record> getBill(QString customerID);
    Slave* findSlave(QString no);
    //void slaveRegister();
    void insertRecord(Slave *slave);                 //向数据库中添加记录
    //void createReport1();           //生成报表一
    //void createReport2();           //生成报表二
    //void createReport3();           //生成报表三
private slots:   
    void getSlaveCuTemperature(const QString no,const double currentTempersture);//当前温度，命令6
    void getSlaveTargetTemperature(const QString no,const int targetTemperature);//目标温度，命令2
    void getSlaveWind(const QString no,const int wind);//风速请求，命令3
    void getSlaveOff(const QString no);//关机请求，命令0
    void getSlaveOn(const QString no,QString ID);//开机请求，命令1
    void getSlaveIsWorking(const QString no);//是否被挂起，图形直接过来
    void getCoston(QString no1)
    {
        Slave *item;
        for(int i=0;i<slavelist->count();i++)
        {
            item = slavelist->at(i);
            //qDebug()<<"qqqqaabbc"<<no1<<item->roomnumber;
            if(no1==item->getno())
            {
                qDebug()<<"qqqq胡旭强";
                server->sendOrder8(no1,item->getTotalCost());
            }
        }
    }

public slots:
    void transport(bool,double,double,int);//接收设置请求
    void recvSet(int,double,double,double,double);//接收主机请求并保存
    void setWorking();

signals:
    void sendSlaveToUI(Slave*);//发送从机参数
    void sendToTcp(bool,double,double,int);
};

#endif // SLAVELIST_H
