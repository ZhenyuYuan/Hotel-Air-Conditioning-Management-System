#include "slavelist.h"
#include "slave.h"
#include "format.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtSql>
#include <QString>
#include <QSqlError>
#include <QDateTime>


slaveList::slaveList()
{
    workingNum=0;
    defaultWind=2;
    slavelist=new QVector<Slave*>;
    server=new Server(this,1234);
    Slave *slave1 = new Slave("311c");
    Slave *slave2 = new Slave("312c");
    Slave *slave3 = new Slave("313c");
    Slave *slave4 = new Slave("315c");
    slavelist->append(slave1);
    slavelist->append(slave2);
    slavelist->append(slave3);
    slavelist->append(slave4);
    connect(server,&Server::powerOff,this,&slaveList::getSlaveOff);
    connect(server,&Server::powerOn,this,&slaveList::getSlaveOn);
    connect(server,&Server::roomTem,this,&slaveList::getSlaveCuTemperature);
    connect(server,&Server::windSpeed,this,&slaveList::getSlaveWind);
    connect(server,&Server::stopWind,this,&slaveList::getSlaveIsWorking);
    connect(server,&Server::targetTem,this,&slaveList::getSlaveTargetTemperature);
    connect(server,&Server::requestCost,this,&slaveList::getCoston);
    //connect(this,SIGNAL(sendToTcp(bool,double,double,int)),server,SLOT(slotsendInitOrder9(bool,double,double,int)));//发给tcp
}

//查找房间
Slave* slaveList::findSlave(QString no)
{
//    Slave* it = slavelist->first();
//    qDebug()<<"第一层输出"<<no;
//    while(it != slavelist->last())
//    {
//        QString num = it->getno();
//        qDebug()<<"第88层输出"<<num;
//        if(num.compare(no) == 0){
//            qDebug()<<"查找成功！";
//            qDebug()<<it->getno();
//            return it;
//            }
//        qDebug()<<"第99层输出"<<num;
//        it++;
//     }
    for(int i=0;i<slavelist->count();i++)
    {
        if(no==slavelist->at(i)->getno())
        {
            qDebug()<<"查找成功！";
            //qDebug()<<it->getno();
            return slavelist->at(i);
        }
    }
    return NULL;
}

//修改当前温度
void slaveList::getSlaveCuTemperature(const QString no, const double currentTempersture)
{
    Slave* it = findSlave(no);
    qDebug() << no;
    if(it != NULL)
    {
        qDebug() << "当前温度赋值成功";

        it->setSlaveCurrenTemperature(currentTempersture);

        switch(it->getwind())
        {
        case 1:
            it->setTotalCost(double(it->lastTemTime.secsTo(QDateTime().currentDateTime()))/180);
            qDebug() << "ADDCOST11";
            break;
        case 2:
            it->setTotalCost(double(it->lastTemTime.secsTo(QDateTime().currentDateTime()))/120);
            qDebug() << "ADDCOST22";
            break;
        case 3:
            it->setTotalCost(double(it->lastTemTime.secsTo(QDateTime().currentDateTime()))/60);
            qDebug() << "ADDCOST33";
            break;
        default:
            break;
        }
        it->lastTemTime=QDateTime().currentDateTime();
        qDebug() << "lasttemtime"<<it->lastTemTime;

        emit sendSlaveToUI(it);
    }
}

//修改目标温度
void slaveList::getSlaveTargetTemperature(const QString no, const int targetTemperature)
{
    Slave* it = findSlave(no);
    if(it != NULL)
    {
        it->setSlaveTargetTemperature(targetTemperature);

        insertRecord(it);

        if(ISDEALING == it->getisWorking())
        {
            //it->setSlaveTargetTemperature(targetTemperature);
            emit sendSlaveToUI(it);
            //it->setSlaveWind(wind);
            //emit sendSlaveToUI(it);
        }
        else
        {
            if(workingNum<3)
            {
                it->setSlaveIsWorking(ISDEALING);
                workingNum++;
                server->sendOrder7(no,1);
                it->beginWorking = QDateTime().currentDateTime();
                //it->setSlaveTargetTemperature(targetTemperature);
                emit sendSlaveToUI(it);
                //insertRecord(it);
            }
            else
            {
                Slave *item;
                int i;
                for(i=0;i<slavelist->count();i++)
                {
                    item = slavelist->at(i);
                    if(no!=item->getno() && item->getwind() < it->getwind())
                    {
                        break;
                    }
                }
                if(4==i)
                {
                    server->sendOrder7(no,0);
                }
                else
                {
                    int wind1,wind2,wind3;
                    item = slavelist->at(0);
                    if(no!=item->getno())
                    {
                        wind1=0;
                        item=slavelist->at(1);
                        if(no!=item->getno())
                        {
                            if(item->getwind() <= slavelist->at(wind1)->getwind())
                            {
                                wind2=1;
                            }
                            else
                            {
                                wind2=wind1;
                                wind1=1;
                            }
                            item=slavelist->at(2);
                            if(no!=item->getno())
                            {
                                if(item->getwind() <= slavelist->at(wind2)->getwind())
                                {
                                    wind3=2;
                                }
                                else
                                {
                                    wind3=wind2;
                                    wind2=2;
                                    if(item->getwind() > slavelist->at(wind1)->getwind())
                                    {
                                        wind2=wind1;
                                        wind1=2;
                                    }
                                }
                            }
                            else
                            {
                                item=slavelist->at(3);
                                if(item->getwind() <= slavelist->at(wind2)->getwind())
                                {
                                    wind3=3;
                                }
                                else
                                {
                                    wind3=wind2;
                                    wind2=3;
                                    if(item->getwind() > slavelist->at(wind1)->getwind())
                                    {
                                        wind2=wind1;
                                        wind1=3;
                                    }
                                }
                            }
                        }
                        else
                        {
                            item=slavelist->at(2);
                            if(item->getwind() <= slavelist->at(wind1)->getwind())
                            {
                                wind2=2;
                            }
                            else
                            {
                                wind2=wind1;
                                wind1=2;
                            }
                            item=slavelist->at(3);
                            if(item->getwind() <= slavelist->at(wind2)->getwind())
                            {
                                wind3=3;
                            }
                            else
                            {
                                wind3=wind2;
                                wind2=3;
                                if(item->getwind() > slavelist->at(wind1)->getwind())
                                {
                                    wind2=wind1;
                                    wind1=3;
                                }
                            }
                        }

                    }
                    else
                    {
                        wind1=1;
                        item=slavelist->at(2);
                        if(item->getwind() <= slavelist->at(wind1)->getwind())
                        {
                            wind2=2;
                        }
                        else
                        {
                            wind1=2;
                            wind2=1;
                        }
                        item=slavelist->at(3);
                        if(item->getwind() <= slavelist->at(wind2)->getwind())
                        {
                            wind3=3;
                        }
                        else
                        {
                            wind3=wind2;
                            wind2=3;
                            if(item->getwind() > slavelist->at(wind1)->getwind())
                            {
                                wind2=wind1;
                                wind1=3;
                            }
                        }
                    }

                    if(slavelist->at(wind3)->getwind()<slavelist->at(wind2)->getwind())
                    {
//                        it->setSlaveIsWorking(ISDEALING);
//                        workingNum++;
//                        server->sendOrder7(slavelist->at(wind3)->getno(),0);
//                        server->sendOrder7(no,1);
//                        it->beginWorking = QDateTime().currentDateTime();
//                        it->setSlaveTargetTemperature(targetTemperature);
//                        emit sendSlaveToUI(it);
                    }
                    else if(slavelist->at(wind2)->getwind()<slavelist->at(wind1)->getwind())//后两个风速相等
                    {
                        if(slavelist->at(wind3)->beginWorking > slavelist->at(wind2)->beginWorking)//wind3开始时间晚于wind2
                        {
                            wind3=wind2;
                        }
                    }
                    else
                    {
                        if(slavelist->at(wind3)->beginWorking > slavelist->at(wind2)->beginWorking)
                        {
                            wind3=wind2;
                        }
                        if(slavelist->at(wind3)->beginWorking > slavelist->at(wind1)->beginWorking)
                        {
                            wind3=wind1;
                        }
                    }
                    it->setSlaveIsWorking(ISDEALING);
                    //workingNum++;
                    slavelist->at(wind3)->setSlaveIsWorking(NODEALING);
                    server->sendOrder7(slavelist->at(wind3)->getno(),0);
                    server->sendOrder7(no,1);
                    it->beginWorking = QDateTime().currentDateTime();
                    //it->setSlaveTargetTemperature(targetTemperature);
                    emit sendSlaveToUI(it);
                    //insertRecord(it);
                }

            }
        }
        //insertRecord(it);
    }
}

//修改风速
void slaveList::getSlaveWind(const QString no, const int wind)
{
    Slave*it = findSlave(no);
    if(it != NULL)
    {
        it->setSlaveWind(wind);
        emit sendSlaveToUI(it);
        insertRecord(it);
    }
}


//从机开机
void slaveList::getSlaveOn(const QString no,QString ID)
{
    qDebug()<<"getslaveon"<<no;
    qDebug()<<"主机收到用户ID为："<<ID;
    Slave * it = findSlave(no);
    qDebug()<<"cddddwe";
    if(it != NULL)
    {
        it->setSlaveOn(OPEN);
        it->setCustomerID(ID);
        emit sendSlaveToUI(it);

        insertRecord(it);

        server->sendInitOrder9(no,workmodeMain,maxTemperatureMain,minTemperatureMain,defTemperatureMain,defaultWind);
        qDebug()<<workmodeMain;
    }
}

//从机关机
void slaveList::getSlaveOff(const QString no)
{
    Slave*it = findSlave(no);
    if(it != NULL)
    {
        it->setSlaveOff(STOP);
        it->setSlaveIsWorking(NODEALING);
        insertRecord(it);

        emit sendSlaveToUI(it);
        workingNum--;
    }
}

//从机是否被挂起
void slaveList::getSlaveIsWorking(const QString no)//主机和从机发起的挂起请求
{
    Slave*it = findSlave(no);
    if(it != NULL)
    {
        it->setSlaveIsWorking(NODEALING);

        insertRecord(it);
        workingNum--;
        server->sendOrder7(no,0);

        emit sendSlaveToUI(it);
    }
}

void slaveList::insertRecord(Slave *slave0)
{  //生成报表和详单
    QSqlQuery query;
    QString customerID = slave0->getCustomerID();
    QString no = slave0->getno();

    //计算状态
    int status;
    if(slave0->getisStart())
        status = (slave0->getisStart()+slave0->getisWorking()) % 2;
    else
        status = 2;

    int targetTemperature = slave0->gettargetTemperature();
    int wind = slave0->getwind();
    QString updateTime = QDateTime::currentDateTime().toString(DATETIMEFORMAT);
    QDateTime UpdateTime;
    UpdateTime = QDateTime::fromString(updateTime,DATETIMEFORMAT);

    int lastStatus;
    int lastWind;
    //int lastTargetTemperature;
    QString lastUpdateTime;

    //double energy;

    query.prepare("insert into report (customerID, roomNum, status, targetTemperature, wind, updateTime) "
                  "values (:customerID, :roomNum, :status, :targetTemperature, :wind, :updateTime)");
    query.bindValue(":customerID", customerID);
    query.bindValue(":roomNum", no);
    query.bindValue(":status", status);
    query.bindValue(":targetTemperature", targetTemperature);
    query.bindValue(":wind", wind);
    query.bindValue(":updateTime", updateTime);

    bool success = query.exec();
    if(!success){
        QSqlError lastError = query.lastError();
        qDebug() << lastError.driverText() << QString(QObject::tr("insert fail!"));
    }

    QSqlQuery query1;
    //query1.prepare("select * from report where customerID = ?");
    //query1.addBindValue(customerID);
    query1.exec("select * from report where customerID = '" + customerID + "'");


    if(query1.last()){
        if(query1.previous()){
            lastStatus = query1.value(2).toInt();
            //lastTargetTemperature = query1.record().value(3).toDouble();
            lastWind = query1.value(4).toInt();

            QDateTime LastUpdateTime;
            LastUpdateTime = QDateTime::fromString(query1.value(5).toString(),DATETIMEFORMAT);
            lastUpdateTime = LastUpdateTime.toString(DATETIMEFORMAT);
            //LastUpdateTime = query1.value(5).toDateTime();

            //qDebug() << "lastStatus"  << lastStatus;

            /*
            if(lastStatus == 1 || lastStatus == 2){
                energy = 0;
                //cost = 0;
            }
            else{
                switch(lastWind){
                case 1:
                    energy = double(LastUpdateTime.secsTo(UpdateTime))/180;
                    qDebug() << "180energy" <<  energy;
                    break;
                case 2:
                    energy = double(LastUpdateTime.secsTo(UpdateTime))/120;
                    qDebug() << "120energy" <<  energy;
                    break;
                case 3:
                    energy = double(LastUpdateTime.secsTo(UpdateTime))/60;
                    qDebug() << "60energy" <<  energy;
                    break;
                default:
                    break;
                }

            }
            */
            //slave0->setTotalCost(cost);
            server->sendOrder8(no,slave0->getTotalCost());

            //qDebug() << lastStatus << "   "<< lastWind;

            double finalTemperature;
            finalTemperature = slave0->getcurrentTemperature();
            double lastTargetTem;
            double lastFinalTem;
            //double lastCost = 0;
            query1.exec("select targetTemperature, finalTemperature, cost from bill where customerID = '" + customerID + "'");

            if(query1.last())
            {
                if(query1.previous()){
                    lastTargetTem = query1.value(0).toDouble();
                    lastFinalTem = query1.value(1).toDouble();
                    //lastCost += query1.value(2).toDouble();
                    qDebug() <<  "lastTargetTem" << lastTargetTem;
                    qDebug() << "lastFinalTem" << lastFinalTem;
                }
            }
            //if(energy != 0){
            QString event;
            if((lastStatus == 1 || lastStatus == 2) && status == 0)
            {
                event = "dealt";
            }
            else if((lastStatus == 1 || lastStatus == 0) && status == 2)
            {
                event = "stop";
            }
            else if(lastStatus ==2 && (status == 1 || status == 0))
            {
                event = "start";
            }
            else if((lastTargetTem != lastFinalTem) && (targetTemperature == finalTemperature)){
                event = "reachTem";
            }





                QSqlQuery query2;
                query2.prepare("insert into bill (customerID, roomNum, startTime, stopTime, wind, targetTemperature, finalTemperature, event, cost) "
                              "values (:customerID, :roomnum, :startTime, :stopTime, :wind, :targetTemperature, :finalTemperature, :event, :cost)");
                query2.bindValue(":customerID", customerID);
                query2.bindValue(":roomnum", no);
                query2.bindValue(":startTime", lastUpdateTime);
                query2.bindValue(":stopTime", updateTime);
                query2.bindValue(":wind", lastWind);
                query2.bindValue(":targetTemperature", targetTemperature);
                query2.bindValue(":finalTemperature", finalTemperature);
                //query2.bindValue(":energy", energy);
                query2.bindValue(":event", event);
                query2.bindValue(":cost", slave0->getTotalCost());



                qDebug() << slave0->getTotalCost();


                bool success1 = query2.exec();
                if(!success1){
                    QSqlError lastError = query.lastError();
                    qDebug() << lastError.driverText() << QString(QObject::tr("insert fail!"));
                }
            //}
        }
    }
}

void slaveList::transport(bool mode,double upper,double lower,int default1)
{
    emit sendToTcp(mode,upper,lower,default1);
}


void slaveList::recvSet(int inMode,double inMin,double inMax,double inDef,double inCost)
{
    //isWorkingMain=inWork;             //是否启动
    workmodeMain=inMode;            //工作模式(制冷、制热)
    minTemperatureMain=inMin;       //最低工作温度
    maxTemperatureMain=inMax;       //最高工作温度
    defTemperatureMain=inDef;       //缺省工作温度
    costPerWMain=inCost;             //单位功率费用
    qDebug()<<"主机设置保存成功！输出示例缺省温度"<<defTemperatureMain;
}

void slaveList::setWorking()
{
    isWorkingMain=true;
    qDebug()<<"主机开机成功！";
}

/*
QVector<record> slaveList::getBill(QString customerID)
{
    for(int i=0;i<slavelist->count();i++)
    {
        if(customerID == slavelist->at(i)->getCustomerID())
        {
            return slavelist->at(i)->createBill();
        }
    }
    return null;
}
*/
