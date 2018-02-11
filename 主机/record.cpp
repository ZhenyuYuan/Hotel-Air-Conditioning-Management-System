#include "record.h"

record::record(QString customerID, QString roomNum, int status, double targetTemperature, int wind, QDateTime updateTime){
    this->customerID = customerID;
    this->roomNum = roomNum;
    this->status = status;
    this->targetTemperature = targetTemperature;
    this->wind = wind;
    this->updateTime = updateTime;
}

void record::setCustomerID(QString customerID){
    this->customerID = customerID;
}

void record::setRoomNum(QString roomNum){
    this->roomNum = roomNum;
}

void record::setStatus(int status){
    this->status = status;
}

void record::setTargetTemperature(double targetTemperature){
    this->targetTemperature = targetTemperature;
}

void record::setWind(int wind){
    this->wind = wind;
}

void record::setUpdateTime(QDateTime updateTime){
    this->updateTime = updateTime;
}

QString record::getCustomerID(){
    return this->customerID;
}

QString record::getRoomNum(){
    return this->roomNum;
}

int record::getStatus(){
    return this->status;
}

double record::getTargetTemperature(){
    return this->targetTemperature;
}

int record::getWind(){
    return this->wind;
}

QDateTime record::getUpdatetime(){
    return this->updateTime;
}
