#include "server.h"
#include <QDebug>
#include <iostream>
using namespace std;

Server::Server(QObject *parent, int port):QTcpServer(parent)
{
    bool testCon;
    testCon=listen(QHostAddress::Any,port);
    cout<<testCon<<"listen success"<<endl;
    //connect(&tcpClientSocketList,&TcpClientSocket::powerOff,this,&Server::slotpowerOff);
}
void Server::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<"startconnect"<<endl;
    TcpClientSocket *tcpClientSocket=new TcpClientSocket(this);
    //connect(tcpClientSocket,SIGNAL(updateClients(QString,int)),this,SLOT(updateClients(QString,int)));
    tcpClientSocket->setSocketDescriptor(socketDescriptor);
    connect(tcpClientSocket,SIGNAL(disconnect(int)),this,SLOT(slotDisconnected(int)));
    connect(tcpClientSocket,&TcpClientSocket::powerOff,this,&Server::slotpowerOff);
    connect(tcpClientSocket,&TcpClientSocket::powerOn,this,&Server::slotpowerOn);
    connect(tcpClientSocket,&TcpClientSocket::targetTem,this,&Server::slottargetTem);
    connect(tcpClientSocket,&TcpClientSocket::windSpeed,this,&Server::slotwindSpeed);
    connect(tcpClientSocket,&TcpClientSocket::stopWind,this,&Server::slotstopWind);
    connect(tcpClientSocket,&TcpClientSocket::requestCost,this,&Server::slotrequestCost);
    connect(tcpClientSocket,&TcpClientSocket::roomTem,this,&Server::slotroomTem);
    tcpClientSocketList.append(tcpClientSocket);
    qDebug()<<"connect success"<<endl;
}

/*
void Server::updateClients(QString msg, int length)
{
    for(int i=0;i<tcpClientSocketList.count();i++)
    {
        QTcpSocket *item = tcpClientSocketList.at(i);
        if(item->write(msg.toLatin1(),length)!=length)
        {
            continue;
        }
    }
}
*/

void Server::slotDisconnected(int descriptor)
{
    cout<<"serverdis"<<endl;
    for(int i=0;i<tcpClientSocketList.count();i++)
    {
        QTcpSocket *item=tcpClientSocketList.at(i);
        if(item->socketDescriptor()==descriptor)
        {
            tcpClientSocketList.removeAt(i);
            return;
        }
    }
    return;
}





