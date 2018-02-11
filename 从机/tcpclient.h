#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
//#include <QHostAddress>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>
class TcpClient : public QTcpSocket
{
    Q_OBJECT
public:
    TcpClient(QString serverHost,int port);
    void dataSend(QJsonObject json);
    void sendOrder0();
    void sendOrder1(QString customerid);
    void sendOrder2(double target);
    void sendOrder3(int wind);
    void sendOrder4();
    void sendOrder5();
    void sendOrder6(double temp);
    void Disconnected();
    void setRoomnumber(QString x)
    {
        roomnumber=x;
    }
    bool getisConnected()
    {
        return isConnected;
    }

private:
    QString lastbuf;
    QString readyHandleBuf;
    bool isConnected;
    int port;
    QString severIP;
    QString roomnumber;
    QString customerId;
    bool handlePackage();//true为可以处理，false为不可处理
private slots:
    void slotDisconnected()
    {
        isConnected=false;
    }
    void dataReceived();
signals:
    void sendDisconnect();
    void onOff(int onoffAC);//温控回复(开始/停止）
    void costIs(double money);//返回费用
    void init(bool mode,double upperTem,double lowerTem,double default1,int winspeed);//初始化
};




#endif // TCPCLIENT_H
