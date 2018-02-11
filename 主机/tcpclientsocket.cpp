#include "tcpclientsocket.h"
#include <iostream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
using namespace std;

TcpClientSocket::TcpClientSocket(QObject *parent)
{
    roomnumber="0";
    connect(this,SIGNAL(readyRead()),this,SLOT(dataReceived()));
    connect(this,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));

}

bool TcpClientSocket::handlePackage()
{
    QString temp=lastbuf;
    lastbuf="";
    qDebug()<<temp<<"qqw";
    int i;
    //char xyz="}";
    for(i=0;i<temp.count();i++)
    {
        //qDebug()<<temp.at(i)<<"999";
        if('}' == temp.at(i))
        {
            break;
        }
    }
    if(i==temp.count()-1)
    {
        readyHandleBuf.append(temp);
        return true;
    }
    else if(i==temp.count())
    {
        lastbuf=temp;
        return false;
    }
    else
    {
        QString a1=temp.left(i+1);
        readyHandleBuf.append(a1);
        lastbuf.append(temp.right(temp.count()-i-1));
        return true;
    }
}

void TcpClientSocket::dataReceived ()
{
    cout<<"received"<<endl;
    while(bytesAvailable()>0)
    {
        int length=bytesAvailable();
        QByteArray buf;
        buf=read(length);

        qDebug()<<buf;

        QString temp(buf.data());
        lastbuf = lastbuf+temp;

        while(handlePackage())
        {
            QByteArray temp1;
            temp1.append(readyHandleBuf);
            QJsonParseError json_error;
            QJsonDocument parse_doucment = QJsonDocument::fromJson(temp1, &json_error);
            QJsonObject json=parse_doucment.object();
            //QJsonValue a=json.value("name");

            //cout<<"jx"<<a.toString().toStdString()<<endl;
            int number=json.value("order").toInt();
            if("0"==roomnumber)
            {
                roomnumber=json.value("number").toString();
                qDebug()<<roomnumber<<"cccv";
            }
            //qDebug()<<"abc"<<number<<"hhhhhhhhhhhhh";
            qDebug()<<"before"<<number;
            switch(number)
            {
                case 0:emit powerOff(json.value("number").toString());
                        break;
                case 1:emit powerOn(json.value("number").toString(),json.value("customerid").toString());
                        qDebug()<<"cddddwe";
                        break;
                case 2:emit targetTem(json.value("number").toString(),json.value("target").toDouble());
                        break;
                case 3:emit windSpeed(json.value("number").toString(),json.value("wind").toInt());
                        break;
                case 4:emit stopWind(json.value("number").toString());
                        break;
                case 5:emit requestCost(json.value("number").toString());
                        break;
                case 6://qDebug()<<"hxqqq"<<json.value("number").toString()<<json.value("temp").toDouble();
                        emit roomTem(json.value("number").toString(),json.value("temp").toDouble());
                        //qDebug()<<"hxqqq";
                        break;
                default:qDebug()<<"Order Error";
            }
            readyHandleBuf="";
        }
        qDebug()<<"reciived!!!";
    }
}

void TcpClientSocket::slotDisconnected()
{
    cout<<"socketdis"<<endl;
    emit disconnect(this->socketDescriptor());
}

void TcpClientSocket::dataSend(QJsonObject data)
{
    int length=0;
    QJsonDocument document;
    document.setObject(data);
    QByteArray byte_array = document.toJson(QJsonDocument::Compact);
    QString data_str(byte_array);
    if((length=this->write(data_str.toLatin1(),data_str.length()))>0)
    {
        qDebug().noquote()<<"Send!!!";
        qDebug()<<data_str<<endl;
        //qDebug().noquote()<<length<<"fason"<<data_str<<endl;
    }
    else
    {
        //cout<<"fail"<<endl;
    }
    this->waitForBytesWritten(1000);
}

void TcpClientSocket::sendOrder7(int response)
{
    QJsonObject json;
    json.insert("response",response);
    json.insert("order",7);
    dataSend(json);
}

void TcpClientSocket::sendOrder8(double cost)
{
    QJsonObject json;
    json.insert("cost",cost);
    json.insert("order",8);
    dataSend(json);
}

void TcpClientSocket::sendOrder9(int mode,double upper,double lower,int default1,int default2)
{
    QJsonObject json;
    json.insert("mode",mode);
    json.insert("upper",upper);
    json.insert("lower",lower);
    json.insert("order",9);
    json.insert("default",default1);
    json.insert("winSpeed",default2);
    dataSend(json);
}


