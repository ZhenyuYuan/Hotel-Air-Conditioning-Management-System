#ifndef MAINFRAMEPARM_H
#define MAINFRAMEPARM_H

#define HIGH 3
#define MEDIUM 2
#define LOW 1

#define STOP 0
#define OPEN 1

#define NODEALING 0
#define ISDEALING 1

#define STOPWIND 0
#define OPENWIND 1

#define COLD 0
#define HOT 1



#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtSql>
#include <QSqlError>

const QString DATETIMEFORMAT="yyyy-MM-dd HH:mm:ss";

typedef struct record
{
       QString customerID;
       QString roomNum;
       QString startTime;
       QString stopTime;
       int wind;
       int targetTemperature;
       double finalTemperature;
       //double energy;
       double cost;
       QString event;
}record;

typedef struct mainFrameParm{
    //mainFrameParm* temFramebool isWorking;             //是否启动（ture开启，false关闭）

    int workmode;          //工作模式(制冷、制热)
    double minTemperature;       //最低工作温度
    double maxTemperature;       //最高工作温度
    double defTemperature;       //缺省工作温度
    int maxRequest=3;             //可处理最多请求数
    double costPerW;             //单位功率费用
}mainFrameParm;

//图形化数据结构，UI过来的从机设置
typedef struct slaveParmUI{
    QString no;
    //bool isStart;//控制从机开关机
    float targetTemperature;
    int wind;
}slaveParm;

//图形化数据结构，传给UI从机当前的空调状态（从机显示）
typedef struct toSlaveParmUI{
    QString no;
    float cost;
    float currentTemperature;
    float targetTemperature;
    int wind;
}toSlaveParmUI;


typedef struct watchToUI{
    QString no;
    int isStart;//开关机
    int isWorking;//是否挂机
    int wind;
    double currentTemperature;
    int targetTemperature;
}watchToUI;


#endif // MAINFRAMEPARM_H

