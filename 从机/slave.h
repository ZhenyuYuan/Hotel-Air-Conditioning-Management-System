#ifndef SLAVE_H
#define SLAVE_H

#include <QString>
#include <QObject>
#include "tcpclient.h"
#include "format.h"

class Slave : public QObject{
    Q_OBJECT
private:
   //TcpClient *tcpclient;
   QString no;
   QString customerID;
   double currentTemperature;
   int targetTemperature;
   int wind;//当前风速
   int isStart;//从机开关机
   int isWorking;//判断从机是否被挂起
   int stopWind;//停风请求
   int workMode;//制冷制热
   double upper,lower;//可以设定的最高温度和最低温度
   double default1;//默认目标温度
   int winSpeed;//默认风速
   double cost;
public:
   TcpClient *tcpclient;
   //Slave();
   Slave(QString no,QString serverHost,int port);

   //~Slave();
   QString getno(){
       return no;
   }
   QString getcustomerID(){
      return  customerID;
   }
   double getcurrentTemperature(){
       return currentTemperature;
   }
   int gettargetTemperature(){
       return targetTemperature;
   }
   int getwind(){
       return wind;
   }
   int getisStart(){
       return isStart;
   }
   int getisWorking(){
       return isWorking;
   }
   int getstopWind(){
       return stopWind;
   }
   int getworkMode(){
       return workMode;
   }
   double getCost(){
       return cost;
   }

   void temperatureModel();//温度模拟，模拟当前温度

private slots:
   void initCustomerId(QString id);
   void initSlaveStatus();//从机开关，图形过来
   void initSlaveTemperature(const int targetTemperature);//图形过来的，从机参数设置，图形过来
   void initSlaveWind(const int wind);//风速设置，图形过来
   void initSlaveCost(const double cost);//主机过来的,从机当前花费。命令8
   void getWorkMode(const int workMode,double upper,double lower,double default1, int winSpeed);//主机过来的工作模式（制冷制热），最高温度，最低温度，命令9
   void slaveWorkSet(const int isWorking);//得到信号：主机允许工作，命令7

signals://信号函数，发送数据
   void gaysendISwork(int isWorking);
   void sendDisconnect();
   void sendCuTemperature(const double Ctemperature);//发送当前温度，命令6以及图形
   void sendTarTemperature(const int Ttemperature);//发送目标温度，命令2以及图形
   void sendwind(const int wind);//发送风速请求，命令3以及图形
   void sendcost(const double cost);//发送费用
   void sendslaveon(const QString customerID);//发送开机请求，命令1
   void sendslaveoff();//发送关机请求，命令0
   void sendstopWind();//发送停风请求，命令4
   void sendWorkMode(const int workMode,double upper,double lower,double default1,int winSpeed);
};

#endif // SLAVE_H


