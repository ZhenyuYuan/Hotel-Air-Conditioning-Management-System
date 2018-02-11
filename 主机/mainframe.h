#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <QObject>
#include <QDebug>
#include "format.h"
#include "slaveList.h"

class mainframe : public QObject{
    Q_OBJECT
private:
    bool isWorking;             //是否启动
    int workmode;          //工作模式(制冷、制热)
    double minTemperature;       //最低工作温度
    double maxTemperature;       //最高工作温度
    double defTemperature;       //缺省工作温度
    double costPerW;             //单位功率费用

    //strategy strategy;          //处理请求的策略
public:
    mainframe(){
        //isWorking = false;
        //workmode = WorkMode::Cold;
        //minTemperature = 15;
        //maxTemperature = 40;
        //defTemperature = 25;
        //maxRequest = 3;
        //strategy = Strategy::FIFS;
    }

    //~mainframe();
public slots:
    void initMainframe(mainFrameParm*);   //槽函数，接收前端的缺省参数配置

    void setWorking();

    int getWorkMode();                     //获取主机工作模式（制冷、制热）

    float getMinTemperature();                  //获取主机工作最小温度

    float getMaxTemperature();                  //获取主机工作最大温度

    float getDefTemperature();                  //获取主机工作缺省温度

    float getCostPerW();                        //获取单位功率费用

    //strategy getStrategy();                     //获取主机调度策略
signals://信号函数，发送数据
   void sendWorkMode(const QString no,const int workmode);//
   void sendMinTemperature(const QString no,const double minTemperature);
   void sendMaxTemperature(const QString no,const double maxTtemperature);
};

#endif // MAINFRAME_H
