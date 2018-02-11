#include "mainwindow.h"
#include"format.h"
#include"setting.h"
#include"slave.h"
#include"slavelist.h"
#include"mainframe.h"
#include <QApplication>
#include <QtSql>
#include <QWidget>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>

extern watchToUI slaveToUI;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //mainframe m;

    //slaveList *slavelist = new slaveList();

    return a.exec();
}
