#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QFile>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile qss(":/qss");
    if (qss.open(QFile::ReadOnly))
    {
        qDebug()<< "yuanzhenyu";
        QString style = QLatin1String(qss.readAll());
        a.setStyleSheet(style);
        qss.close();
    }
    else
    {
        qDebug()<<"fail";
    }
    MainWindow w;
    w.show();
    w.slave->temperatureModel();
    return a.exec();
}
