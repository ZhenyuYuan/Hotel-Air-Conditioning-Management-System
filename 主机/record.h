#ifndef REPORT_H
#define REPORT_H

#include <QString>
#include <QObject>
#include <QDateTime>
#include "format.h"

class record : public QObject{
     Q_OBJECT
private:
     QString customerID;
     QString roomNum;
     int status;
     double targetTemperature;
     int wind;
     QDateTime updateTime;
public:
     record(QString customerID, QString roomNum, int status, double targetTemperature, int wind, QDateTime updateTime);
     //~record();

     void setCustomerID(QString customerid);
     void setRoomNum(QString roomnum);
     void setStatus(int status);
     void setTargetTemperature(double curTemperature);
     void setWind(int wind);
     void setUpdateTime(QDateTime updateTime);

     QString getCustomerID();
     QString getRoomNum();
     int getStatus();
     double getTargetTemperature();
     int getWind();
     QDateTime getUpdatetime();
};


#endif // REPORT_H
