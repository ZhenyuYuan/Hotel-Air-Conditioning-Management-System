#ifndef SLAVE_H
#define SLAVE_H

#include <QString>
#include <QObject>
#include <QDateTime>
#include "format.h"
//#include "tcpclientsocket.h"
class Slave : public QObject
{
    Q_OBJECT
private:
   QString no;//房间号
   QString customerID;
   double currentTemperature;
   int targetTemperature;
   int wind;
   int isStart;//从机开关机
   int isWorking;//判断从机是否被挂起
   int stopWind;//停风请求
   int workMode;//制冷制热


   //QVector<record> bill;    //存详单


   double totalCost;
public:


   QDateTime beginWorking;//这一次开始吹风的时间
   QDateTime lastTemTime;//上一次加入费用的时间
   //TcpClientSocket *tcpClientSocket;
   Slave(QString no);

   //~Slave();
   QString getno(){
       return no;
   }
   QString getCustomerID(){
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
   double getTotalCost(){
       return totalCost;
   }

   void setSlaveCurrenTemperature(double CuTemperature){
      // qDebug()<<"到了setSlaveCurrenTemperature";
       currentTemperature = CuTemperature;
   }

   void setSlaveTargetTemperature(int taTemperature){
       targetTemperature = taTemperature;
   }

   void setSlaveWind(int wind){
       this->wind = wind;
   }

   void setSlaveOff(int stop){
       isStart = stop;
   }

   void setSlaveOn(int on){
       isStart = on;
   }

   void setCustomerID(QString inID)
   {
       customerID=inID;
   }

   void setSlaveIsWorking(int isdealing){
       isWorking = isdealing;
   }

   //void setRecord(QString customerID, QDateTime startTime, QDateTime stopTime, int wind, int targetTemperature, double finalTemperature, double energy, double cost);

   void setTotalCost(double cost);

   QVector<record> createBill();
   //void temperatureModel();//温度模拟，模拟当前温度

private slots:
   void initCustomerId(QString id);
   //void initSlaveStatus();//从机开关，图形过来
   //void initSlaveTemperature(const int targetTemperature);//图形过来的，从机参数设置，图形过来
   //void initSlaveWind(const int wind);//风速设置，图形过来
   //void initSlaveCost(const double cost);//主机过来的,从机当前花费。命令8
   void getWorkMode(const int workMode);//主机过来的工作模式（制冷制热），命令9
   //void slaveWorkSet(const int isWorking);//得到信号：主机允许工作，命令7

signals://信号函数，发送数据
//   void sendCuTemperature(const QString no,const double Ctemperature);//发送当前温度，命令6以及图形
//   void sendTarTemperature(const QString no,const int Ttemperature);//发送目标温度，命令2以及图形
//   void sendwind(const QString no,const int wind);//发送风速请求，命令3以及图形
//   void sendcost(const QString no,const float cost);//发送费用
//   void sendslaveon(const QString no,const int isStart,const QString customerID);//发送开机请求，命令1
//   void sendslaveoff(const QString no,const int isStart);//发送关机请求，命令0
//   void sendstopWind(const QString no,const int stopWind);//发送停风请求，命令4
};

#endif // SLAVE_H


