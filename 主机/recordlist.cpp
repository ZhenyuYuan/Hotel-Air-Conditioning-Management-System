#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtSql>
#include <QString>
#include <QSqlError>
#include <fstream>

#include "record.h"
#include "recordList.h"


void recordList::selectRecord(QString customerID){
    QSqlQuery query;
    query.exec("select customerID from history where customerID ="+customerID );
    while(query.next()) {
        QString customerID = query.value(0).toString();
        QString roomNum = query.value(1).toString();
        int status = query.value(2).toInt();
        double targetTemperature = query.value(3).toInt();
        int wind =query.value(4).toInt();
        QDateTime updateTime = query.value(5).toDateTime();

        record *rec = new record(customerID, roomNum, status, targetTemperature, wind, updateTime);
        recordlist.append(rec);
    }
}


//生成详单
void recordList::createdetail()
{
    int i;
    int note;
    struct dtl d;
    note = 0;
    for(i=1; i<=recordlist.size()-1; i++)
    {
        if((recordlist[note]->getStatus() != recordlist[i]->getStatus())||(recordlist[note]->getWind() != recordlist[i+1]->getWind()))
        {
            d.roomNum = recordlist[i]->getRoomNum();
            d.start = recordlist[note]->getUpdatetime();
            d.end = recordlist[i]->getUpdatetime();
            d.status = recordlist[note]->getStatus();
            if(recordlist[note]->getStatus()==1 || recordlist[note]->getStatus()==2)
            {//状态为关机或停风不机计费
                d.energy = 0;
                d.cost = 0;
            }
            else{
            d.wind = recordlist[note]->getWind();
            switch (d.wind) {
            case 1:
                //d.energy = (d.end - d.start)*1;
                d.energy = d.start.secsTo(d.end)*1;
                break;
            case 2:
                d.energy = d.start.secsTo(d.end)*2;
            case 3:
                d.energy = d.start.secsTo(d.end)*3;
                break;
            default:
                break;
            }
            d.cost = d.energy*1;
            }

            detailList.push_back(d);
            note = i;
        }

    }

    //将详单存入数据库
    QSqlQuery query;
    for(i=0; i<=detailList.size()-1; i++){
        query.prepare("insert into bill (customerID, roomnum, start, end, status, wind, energy, cost) values (:a, :b, :c, :d, :e, :f, :g, :h)");
        query.bindValue(":a", customerID);
        query.bindValue(":b", detailList[i].roomNum);
        query.bindValue(":c", detailList[i].start);
        query.bindValue(":d", detailList[i].end);
        query.bindValue(":e", detailList[i].status);
        query.bindValue(":f", detailList[i].wind);
        query.bindValue(":g", detailList[i].energy);
        query.bindValue(":h", detailList[i].cost);
    }

/*
    //生成详单文件
    File fp("detail.txt");
    if (fp.open(QFile::WriteOnly | QIODevice::Truncate)) //覆盖写入
    {
        QTextStream out(&fp);
        out<<"客户姓名"<< cid<<endl;
        out<<"房间号"<<endl;
        out<<"开始时间\t"<<"结束时间\t"<<"状态\t"<<"风速\t"<<"功耗\t"<<"金额\t"<<endl;
        for(i=0; i<=detailList.size()-1; i++)
            out << detailList[i].start << "\t" << detailList[i].end<<"\t"
                << detailList[i].status << "\t" << detailList[i].wind
                << "\t" << detailList[i].energy << "\t"<<detailList[i].cost;
    }
    */
}

//生成账单
double recordList::createbill()
{
    recordlist.clear();
    int i;
    int note;
    double totalenergy, totalcost;
    totalenergy = 0;
    note = 0;
    for(i=1; i<=recordlist.size()-1; i++)
    {
        if((recordlist[note]->getStatus() != recordlist[i]->getStatus()) || (recordlist[note]->getWind() != recordlist[i+1]->getWind()))
        {
            if(recordlist[note]->getStatus()==1 || recordlist[note]->getStatus()==2)
            {//状态为关机或停风不机计费
               totalenergy = totalenergy;
            }
            else{
            switch (recordlist[note]->getWind()) {
            case 1:
                totalenergy = totalenergy + recordlist[note]->getUpdatetime().secsTo(recordlist[i]->getUpdatetime())*1;
                break;
            case 2:
                totalenergy = totalenergy + recordlist[note]->getUpdatetime().secsTo(recordlist[i]->getUpdatetime())*2;
                break;
            case 3:
                totalenergy = totalenergy + recordlist[note]->getUpdatetime().secsTo(recordlist[i]->getUpdatetime())*3;
                break;
            default:
                break;
            }

            note = i;
            }
        }
    }
    totalcost = totalenergy*1;
    return totalcost;
}

void recordList::senddtl(QVector<dtl> detailList)
{
    emit detail(detailList);
}

void recordList::sendcost(double totalcost)
{
    emit cost(totalcost);
}
