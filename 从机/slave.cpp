#include "slave.h"
#include "format.h"

#include <QDate>
#include <QCoreApplication>
#include <QVector>
#include <QApplication>
#include <math.h>

Slave::Slave(QString no,QString serverHost,int port)
{
    tcpclient=new TcpClient(serverHost,port);
    this->no = no;
    this->customerID = "";
    this->currentTemperature = 24;
    this->targetTemperature = 25;
    this->wind = STOP;
    this->isStart = STOP;
    this->isWorking = NODEALING;
    this->stopWind = STOPWIND;
    this->cost = 0.0;
    this->workMode = 0;
    tcpclient->setRoomnumber(no);
    connect(this,&Slave::sendCuTemperature,tcpclient,&TcpClient::sendOrder6);
    connect(this,&Slave::sendTarTemperature,tcpclient,&TcpClient::sendOrder2);
    connect(this,&Slave::sendwind,tcpclient,&TcpClient::sendOrder3);
    //connect(this,&Slave::sendcost,tcpclient,&TcpClient::sendOrder5);
    connect(this,&Slave::sendslaveon,tcpclient,&TcpClient::sendOrder1);
    connect(this,&Slave::sendslaveoff,tcpclient,&TcpClient::sendOrder0);
    connect(this,&Slave::sendstopWind,tcpclient,&TcpClient::sendOrder4);
    connect(tcpclient,&TcpClient::costIs,this,&Slave::initSlaveCost);
    connect(tcpclient,&TcpClient::onOff,this,&Slave::slaveWorkSet);
    connect(tcpclient,&TcpClient::init,this,&Slave::getWorkMode);
    connect(tcpclient,&TcpClient::sendDisconnect,this,&Slave::sendDisconnect);
    //temperatureModel();
}
//开关机
void Slave::initSlaveStatus(){
    this->isStart = !this->isStart;
    qDebug()<<"hxq"<<isStart;
    if(this->isStart == 0)
        emit sendslaveoff();
    else
        emit sendslaveon(customerID);
}

//获得customerID
void Slave::initCustomerId(QString no){
    this->customerID = no;
}

//从界面过来的命令，从机目标温度设置
void Slave::initSlaveTemperature(const int targetTemperature){
    this->targetTemperature = targetTemperature;
    emit sendTarTemperature(targetTemperature);
}

//从机风速设置
void Slave::initSlaveWind(const int wind){
    this->wind = wind;
    //qDebug()<<"changeWind"<<

    emit sendwind(wind);
}

//从主机过来的从机当前情况（花费）
void Slave::initSlaveCost(const double cost){
    this->cost = cost;
    emit sendcost(cost);
}

void Slave::getWorkMode(const int workMode,double upper,double lower,double default1,int winspeed)
{
    this->workMode = workMode;
    this->upper = upper;
    this->lower = lower;
    this->default1 = default1;
    this->winSpeed = winspeed;
    emit sendWorkMode(workMode, upper, lower,default1,winspeed);
}

void Slave::slaveWorkSet(const int isDealing){
    this->isWorking = isDealing;
    emit gaysendISwork(isWorking);
}

//温度模拟
void Slave::temperatureModel(){
    QTime t;
    int s;
    double derta;//变化的幅度
    emit sendCuTemperature(currentTemperature);
    while(1)
    {
        s = 60;
        t.start();
        while(t.elapsed() < 1000 * s)
        {//每30秒变化一次
            QCoreApplication::processEvents();

        }
        //tcpclient->sendOrder5();
        qDebug()<<"1234";
        if(isWorking == ISDEALING && isStart == OPEN)
        {//开启空调模式
            switch(wind)
            {
                case HIGH:derta=1;
                    break;
                case MEDIUM:derta=0.5;
                    break;
                case LOW:derta=1.0/3;
                    break;
                default:derta=0;
                    break;
            }
            qDebug()<<"derta "<<derta;
            if(this->workMode == HOT)
            {//吹热风
                if(currentTemperature < targetTemperature)
                {
                    currentTemperature = currentTemperature + derta;
                    emit sendCuTemperature(currentTemperature);
                    qDebug()<<"56";
                }
                //qDebug()<<"opcu-tar"<<currentTemperature-targetTemperature;
                if(currentTemperature >= targetTemperature || fabs(currentTemperature-targetTemperature)<pow(10,-4))
                {
                    //qDebug()<<"cu-tar"<<abs(currentTemperature-targetTemperature)<<"pow"<<pow(10,-4);
                    emit sendstopWind();
                    slaveWorkSet(NODEALING);
                }
            }
            else
            {//吹冷风
                if(currentTemperature > targetTemperature)
                {
                    currentTemperature = currentTemperature - derta;
                    emit sendCuTemperature(currentTemperature);
                    qDebug()<<"78";
                }
                //qDebug()<<"opcu-tar"<<currentTemperature-targetTemperature;
                if(currentTemperature <= targetTemperature || fabs(currentTemperature-targetTemperature)<pow(10,-4))
                {
                    //qDebug()<<"cu-tar"<<currentTemperature-targetTemperature<<"pow"<<pow(10,-4);
                    emit sendstopWind();
                    slaveWorkSet(NODEALING);
                }
            }
        }
        //开启室温变化模式
        else
        {
            if(workMode == HOT)
            {//冬天
                if(currentTemperature >= 24)
                {//没有温度
                    currentTemperature = currentTemperature - 0.5;
                    emit sendCuTemperature(currentTemperature);
                    qDebug()<<"910";
                    double x=currentTemperature-targetTemperature;
                    qDebug()<<"x:"<<x;
                    if(x<=-1 && isStart == OPEN)
                    {
                        emit sendTarTemperature(targetTemperature);
                    }
                    qDebug()<<"*****";
                }
            }
            else
            {//夏天
                if(currentTemperature < 24)
                {
                    currentTemperature = currentTemperature + 0.5;
                    emit sendCuTemperature(currentTemperature);
                    qDebug()<<"1112";

                    double x=currentTemperature-targetTemperature;
                    if(x>=1 && isStart == OPEN)
                    {
                        emit sendTarTemperature(targetTemperature);
                    }
                }
            }  
        }
        tcpclient->sendOrder5();
    }
}
