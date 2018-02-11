#ifndef HISTORYLIST_H
#define HISTORYLIST_H

#include <QDateTime>
#include <QString>

#include "record.h"

class recordList: public QObject{
    Q_OBJECT

private:
    QVector<record*> recordlist;
    QString customerID;
public:
    recordList();
    //~recordList();
    struct dtl{
        //QString customerId;
        QString roomNum; //房间号
        QDateTime start;    //开始时间
        QDateTime end;     //结束时间
        int status;     //状态
        //double currenttemprature;
        int wind;      //风速

        double energy;
        double cost;
    };
    QVector<dtl> detailList;    //存详单
    void selectRecord(QString customerID);


    void createdetail();//生成详单
    double createbill();//生成账单
    void senddtl(QVector<dtl> detailList);
    void sendcost(double totalcost);

    //void appendRecord(record rec);
signals:
    void detail(QVector<dtl> detailList);
    void cost(double totalcost);
};

#endif // HISTORYLIST_H
