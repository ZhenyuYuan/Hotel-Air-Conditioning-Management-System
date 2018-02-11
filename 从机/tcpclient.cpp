#include "tcpclient.h"
#include <iostream>
#include <QCoreApplication>
#include <windows.h>
using namespace std;


TcpClient::TcpClient(QString serverHost,int port)
{
    qDebug()<<"question2";
    //port=1234;
    //severIP->setAddress("127.0.0.1");
    qDebug()<<"question3";
    roomnumber="";
    customerId="aaa";
    connectToHost(serverHost,port);


//    qDebug()<<serverHost<<port<<" 9985";
//    if(waitForConnected(1000))
//    {
//        qDebug()<<"aassrew";
//        isConnected=true;
//    }


    int i;
    QCoreApplication::processEvents();
    Sleep(1000);

    for(i=0;i<3 && (!waitForConnected(1000));i++)
    {
        qDebug()<<"aaaaaaac";
        connectToHost("10.110.211.93",port);
    }
    if(3==i)
    {
        isConnected=false;
        //emit sendDisconnect();
    }
    else
    {
        isConnected=true;
    }


    qDebug()<<"question4";
    connect(this,&QTcpSocket::readyRead,this,&TcpClient::dataReceived);
    connect(this,&QTcpSocket::disconnected,this,&TcpClient::sendDisconnect);
    connect(this,&QTcpSocket::disconnected,this,&TcpClient::slotDisconnected);
    qDebug()<<"question5";
}

bool TcpClient::handlePackage()
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

void TcpClient::dataReceived ()
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
                case 7:emit onOff(json.value("response").toInt());
                        break;
                case 8:emit costIs(json.value("cost").toDouble());
                        break;
                case 9:emit init(json.value("mode").toInt(),json.value("upper").toDouble(),json.value("lower").toDouble(),json.value("default").toDouble(),json.value("winSpeed").toInt());
                        break;
                default:qDebug()<<"Order Error";
            }
            readyHandleBuf="";
        }
        qDebug()<<"reciived!!!";
    }
}

/*
void TcpClient::dataReceived()
{
    //cout<<"received"<<endl;
    while(bytesAvailable()>0)
    {
        int length=bytesAvailable();
        QByteArray buf;
        buf=read(length);

        qDebug().noquote()<<buf;
        cout<<buf.data();

        QJsonParseError json_error;
        QJsonDocument parse_doucment = QJsonDocument::fromJson(buf, &json_error);
        QJsonObject json=parse_doucment.object();
        QJsonValue a;
        //cout<<"jx"<<a.toString().toStdString()<<endl;


        a=json.value("name");
        qDebug()<<"reciived!!!";
        qDebug()<<"name"<<a.toString();
        a=json.value("time");
        qDebug()<<"time"<<a.toString()<<endl;


        int number=json.value("order").toInt();
        switch(number)
        {
            case 7:emit onOff(json.value("response").toInt());
                    break;
            case 8:emit costIs(json.value("cost").toDouble());
                    break;
            case 9:emit init(json.value("mode").toInt(),json.value("upper").toDouble(),json.value("lower").toDouble(),json.value("default").toDouble(),json.value("winSpeed").toInt());
                    break;
            default:qDebug()<<"Order Error";
        }

        //QString msg=buf.to;
        //cout<<buf<<endl;
        //emit updateClients(msg,length);
    }
}
*/

void TcpClient::dataSend(QJsonObject data)
{
    int length=0;
    QJsonDocument document;
    document.setObject(data);
    QByteArray byte_array = document.toJson(QJsonDocument::Compact);
    QString data_str(byte_array);
    if((length=this->write(data_str.toLatin1(),data_str.length()))>0)
    {
        //QString abc="qqqqqq";
        //qDebug()<<abc;
        qDebug().noquote()<<"Send!!!";
        qDebug()<<data_str<<endl;
        cout<<"666"<<endl;
        cout<<data_str.toStdString()<<endl;
    }
    else
    {
        //cout<<"fail"<<endl;
    }
    this->waitForBytesWritten(1000);
}

void TcpClient::Disconnected()
{
    this->disconnectFromHost();
}

void TcpClient::sendOrder0()
{
     QJsonObject json;
     json.insert("number",roomnumber);
     json.insert("order",0);
     dataSend(json);
}

void TcpClient::sendOrder1(QString customerId)
{
     QJsonObject json;
     json.insert("number",roomnumber);
     json.insert("order",1);
     json.insert("customerid",customerId);
     dataSend(json);
}

void TcpClient::sendOrder2(double target)
{
     QJsonObject json;
     json.insert("number",roomnumber);
     json.insert("order",2);
     json.insert("target",target);
     dataSend(json);
}

void TcpClient::sendOrder3(int wind)
{
     QJsonObject json;
     json.insert("number",roomnumber);
     json.insert("order",3);
     json.insert("wind",wind);
     dataSend(json);
}

void TcpClient::sendOrder4()
{
     QJsonObject json;
     json.insert("number",roomnumber);
     json.insert("order",4);
     dataSend(json);
}

void TcpClient::sendOrder5()
{
     QJsonObject json;
     json.insert("number",roomnumber);
     json.insert("order",5);
     dataSend(json);
}

void TcpClient::sendOrder6(double temp)
{
     QJsonObject json;
     json.insert("number",roomnumber);
     json.insert("order",6);
     json.insert("temp",temp);
     dataSend(json);
}
