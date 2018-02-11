#include "mainframe.h"
#include "format.h"
#include <QDebug>

void mainframe::initMainframe(mainFrameParm * mainFrameIN)
{    //槽函数，接收前端的缺省参数配置
    //this->isWorking = mainFrameIN->isWorking;                  //主机开机标识
    this->workmode = mainFrameIN->workmode;                    //主机工作模式（制冷、制热）
    this->minTemperature = mainFrameIN->minTemperature;        //最低工作温度
    this->maxTemperature = mainFrameIN->maxTemperature;        //最高工作温度
    this->defTemperature = mainFrameIN->defTemperature;        //缺省工作温度
    //this->maxRequest = mainFrameParm.maxRequest;                //可处理最多请求数
    this->costPerW = mainFrameIN->costPerW;                    //单位功率费用
    //this->currentRequest = 0;                                   //当前请求数
    //this->strategy = strategy::HP;                              //处理请求的策略


    emit sendWorkMode("818",workmode);
    emit sendMinTemperature("818",minTemperature);
    emit sendMaxTemperature("818",maxTemperature);
    qDebug()<<"测试工作模式"<<mainFrameIN->workmode;
    qDebug()<<"功率费用"<<mainFrameIN->costPerW;
    qDebug()<<"最大温度"<<mainFrameIN->maxTemperature;
}

void mainframe::setWorking(){
    this->isWorking = true;
}

int mainframe::getWorkMode(){          //获取主机工作模式（制冷、制热）
    return this->workmode;
}

float mainframe::getMinTemperature(){       //获取主机工作最小温度
    return this->minTemperature;
}

float mainframe::getMaxTemperature(){       //获取主机工作最大温度
    return this->maxTemperature;
}

float mainframe::getDefTemperature(){       //获取主机工作缺省温度
    return this->defTemperature;
}

float mainframe::getCostPerW(){             //获取单位功率费用
    return this->costPerW;
}

/*strategy mainframe::getStrategy(){          //获取主机调度策略
    return this->strategy;
}*/

