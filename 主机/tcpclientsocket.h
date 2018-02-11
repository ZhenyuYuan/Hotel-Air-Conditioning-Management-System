#ifndef TCPCLIENTSOCKET_H
#define TCPCLIENTSOCKET_H

#include <QTcpSocket>
#include <QObject>

class TcpClientSocket : public QTcpSocket
{
    Q_OBJECT

public:
    QString roomnumber;
    TcpClientSocket(QObject *parent=0);
    void sendOrder7(int response);
    void sendOrder8(double cost);
    void sendOrder9(int mode,double upper,double lower,int default1,int default2);
    //void
private:
    QString lastbuf;
    QString readyHandleBuf;
    QString customerId;
    bool handlePackage();
    void dataSend(QJsonObject data);
signals:
    void powerOff(QString number);
    void powerOn(QString number,QString customerid);
    void targetTem(QString number,double target);
    void windSpeed(QString number,int wind);
    void stopWind(QString number);
    void requestCost(QString number);
    void roomTem(QString number,double temp);
    //void updateClients(QString,int);
    void disconnect (int);
protected slots:
    void dataReceived();
    void slotDisconnected();
};


#endif // TCPCLIENTSOCKET_H
