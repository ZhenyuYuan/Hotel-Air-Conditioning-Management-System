#include <QtSql>
#include <QWidget>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFile>

//#include "slave.h"
#include "mainwindow.h"


void MainWindow::createConnection(bool isExist){
    if(!db.open()){
        //如果数据库未打开，则报错
        QMessageBox::critical(0, QObject::tr("数据库错误"), db.lastError().text());
        return;
    }
    QSqlQuery query;
    if(!isExist){
        query.exec("create table report ("
                   "customerID navachar,"
                   "roomNum navachar,"
                   "status int,"
                   "targetTemperature int,"
                   "wind int,"
                   "updateTime navachar)"
                   );

        query.exec("create table bill ("
                   "customerID nvarchar,"
                   "roomNum nvarchar,"
                   "startTime navachar,"
                   "stopTime navachar,"
                   "wind int,"
                   "targetTemperature int,"
                   "finalTemperature double,"
                   "event navacharenergy real,"
                   "cost real)"
                   );
    }
    else
        qDebug() << QObject::tr("数据库已存在！");
}
