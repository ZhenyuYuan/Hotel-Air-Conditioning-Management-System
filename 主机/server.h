#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QObject>

#include "tcpclientsocket.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    void sendInitOrder9(QString no1,int mode,double upper,double lower,int default1,int default2)
    {
        for(int i=0;i<tcpClientSocketList.count();i++)
        {
            TcpClientSocket *item = tcpClientSocketList.at(i);
            qDebug()<<"qqqqaabbc"<<no1<<item->roomnumber;
            if(no1==item->roomnumber)
            {
                qDebug()<<"qqqq胡旭强";
                item->sendOrder9(mode,upper,lower,default1,default2);
            }
        }
    }
    void sendOrder7(QString no1,int response)
    {
        for(int i=0;i<tcpClientSocketList.count();i++)
        {

            TcpClientSocket *item = tcpClientSocketList.at(i);
            if(no1==item->roomnumber)
            {

                item->sendOrder7(response);
            }
        }
    }
    void sendOrder8(QString no1,double cost)
    {
        for(int i=0;i<tcpClientSocketList.count();i++)
        {
            TcpClientSocket *item = tcpClientSocketList.at(i);
            if(no1==item->roomnumber)
            {
                item->sendOrder8(cost);
            }
        }
    }
    Server(QObject *parent=0,int port=0);
    QList<TcpClientSocket*> tcpClientSocketList;
signals:
    void powerOff(QString number);
    void powerOn(QString number,QString customerid);
    void targetTem(QString number,double target);
    void windSpeed(QString number,int wind);
    void stopWind(QString number);
    void requestCost(QString number);
    void roomTem(QString number,double temp);
    //void
    //void updateServer(QString,int);
public slots:


    void slotpowerOff(QString number)
    {
        emit powerOff(number);
    }
    void slotpowerOn(QString number,QString customerid)
    {
        qDebug()<<"slotpoweron";
        emit powerOn(number,customerid);
    }
    void slottargetTem(QString number,double target)
    {
        emit targetTem(number,target);
    }
    void slotwindSpeed(QString number,int wind)
    {
        emit windSpeed(number,wind);
    }
    void slotstopWind(QString number)
    {
        emit stopWind(number);
    }
    void slotrequestCost(QString number)
    {
        emit requestCost(number);
    }
    void slotroomTem(QString number,double temp)
    {
        qDebug()<<"slotroomtem";
        emit roomTem(number,temp);
    }

    //void updateClients(QString,int);
    void slotDisconnected(int);
protected:
    void incomingConnection(qintptr socketDescriptor);


};


#endif // SERVER_H
