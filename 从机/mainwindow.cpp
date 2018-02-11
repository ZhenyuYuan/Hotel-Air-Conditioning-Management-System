#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logindialog.h"
#include "ui_logindialog.h"
#include <qdebug.h>
#include "slave.h"
#include "ipdialog.h"
#include <QDateTime>
#include <QTimer>
#include "tcpclient.h"
#include "disconnectdialog.h"
#include "connectdialog.h"
#include <windows.h>
#include <QCoreApplication>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->setWindowFlags(this->windowFlags()|Qt::FramelessWindowHint);

    workMode = 0;
    upper = 10;
    lower = 0;

    IPDialog *ipdialog = new IPDialog();
    ipdialog->setFixedSize(ipdialog->width(), ipdialog->height());
    ipdialog->exec();

    connectDialog *con = new connectDialog();
    con->show();

    slave = new Slave("314c",ipdialog->getSeverHost(),ipdialog->getPort());//69

    ipdialog->done(0);
    ipdialog->~IPDialog();

    QObject::connect(slave->tcpclient, &TcpClient::sendDisconnect, this, &MainWindow::showDisCon);

    if (slave->tcpclient->getisConnected() == false)
    {

        qDebug()<<slave->tcpclient->getisConnected();
        con->done(0);
        disconnectDialog *a = new disconnectDialog();
        a->exec();
    }

    con->success();
    QCoreApplication::processEvents();
    Sleep(1000);
    con->done(0);
    con->~connectDialog();

    QObject::connect(slave,  SIGNAL(sendWorkMode(int,double,double,double,int)), this, SLOT(getWorkMode(int,double,double,double,int)));

    login = new loginDialog();
    QObject::connect(login,  SIGNAL(getCusID(QString)), slave, SLOT(initCustomerId(QString)));
    QObject::connect(login,  SIGNAL(getCusID(QString)), slave, SLOT(initSlaveStatus()));
    login->setFixedSize(login->width(), login->height());
    login->exec();

    ui->setupUi(this);
    ui->label_3->setText(slave->getcustomerID());
    this->setFixedSize(this->width(), this->height());

    ui->roomNumLabel->setStyleSheet("font-size:16px");
    ui->roomNumLabel->setText("房间 314c");

    wind = 2;
    ui->windNumLabel->setText("中风");
    //sendWindSpeed(wind);

    ui->label->setText(QString::number(lower) + "\342\204\203");//摄氏度符号
    tem = lower;
    //sendTarTem(tem);

    if (workMode == 0)
    {
        ui->workType->setText("制冷");
    }
    else ui->workType->setText("制热");

    ui->temNumLabel->setText("");

    ui->payNumLabel->setText("0\345\205\203");//元

    ui->lcdNumber->setDigitCount(8);
    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);

    QTimer *t = new QTimer(this);
    t->setInterval(1000);


    QObject::connect(t, SIGNAL(timeout()), this, SLOT(onTimeOut()));
    QObject::connect(this,  SIGNAL(tarTem(int)), slave, SLOT(initSlaveTemperature(int)));
    QObject::connect(slave,  SIGNAL(sendCuTemperature(double)), this, SLOT(getData(double)));
    QObject::connect(slave,  SIGNAL(sendcost(double)), this, SLOT(getCost(double)));
    QObject::connect(this, SIGNAL(touchSwitch()), slave, SLOT(initSlaveStatus()));
    QObject::connect(this, SIGNAL(windSpeed(int)), slave, SLOT(initSlaveWind(int)));
    QObject::connect(login,  SIGNAL(getCusID(QString)), this, SLOT(show()));
    QObject::connect(login,  SIGNAL(getCusID(QString)), ui->label_3, SLOT(setText(QString)));
    QObject::connect(slave, SIGNAL(gaysendISwork(int)), this, SLOT(getIsWork(int)));
    //QObject::connect(slave, SIGNAL(sendCuTemperature()))


    t->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::test(int i)
{
    qDebug() << "You clicked me!" << i;
}

void MainWindow::getData(double i)
{
    ui->temNumLabel->setText(QString::number(i,'f',2) + "\342\204\203");//摄氏度符号
}

/*void MainWindow::touchSwitch(int a)
{

}
*/

void MainWindow::on_switchButton_clicked()
{
//    QIcon icon[2];
//    QPixmap pix[2];
//    pix[0].load(":/tu1");
//    pix[1].load(":/tu2");
//    icon[0].addPixmap(pix[0]);
//    icon[1].addPixmap(pix[1]);
//    i = !i;
//    ui->switchButton->setIcon(icon[i]);
//    ui->switchButton->setIconSize(QSize(100, 100));
    this->hide();
    login->show();
    send();
}

void MainWindow::on_toolButton_clicked()
{
    if (wind != 3)
    {
        wind++;
        if (wind == 2)
            ui->windNumLabel->setText(QObject::tr("中风"));
        if (wind == 3)
            ui->windNumLabel->setText("大风");
        sendWindSpeed(wind);
    }

}



void MainWindow::on_toolButton_2_clicked()
{
    if (wind != 1)
    {
        wind--;
        if (wind == 2)
            ui->windNumLabel->setText("中风");
        if (wind == 1)
            ui->windNumLabel->setText("小风");
        sendWindSpeed(wind);
    }

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    exit(0);
}

void MainWindow::getWorkMode(const int workMode, double upper, double lower ,double default1, int wind)
{
    this->workMode = workMode;
    this->upper = upper;
    this->lower = lower;
    tem = default1;
    ui->label->setText(QString::number(tem) + "\342\204\203");
    if (workMode == 0)
    {
        ui->workType->setText("制冷");
    }
    else ui->workType->setText("制热");
    this->wind = wind;
    if (wind == 3)
        ui->windNumLabel->setText("大风");
    if (wind == 2)
        ui->windNumLabel->setText("中风");
    if (wind == 1)
        ui->windNumLabel->setText("小风");
    sendWindSpeed(wind);
    sendTarTem(tem);
    sendWindSpeed(wind);
}

void MainWindow::on_toolButton_3_clicked()
{
    if (tem <upper)
    {
        tem++;
        ui->label->setText(QString::number(tem) + "\342\204\203");//摄氏度符号
        sendTarTem(tem);
    }
}

void MainWindow::on_toolButton_4_clicked()
{
    if (tem >lower)
    {
        tem--;
        ui->label->setText(QString::number(tem) + "\342\204\203");//摄氏度符号
        sendTarTem(tem);
    }
}

void MainWindow::onTimeOut()
{
    QDateTime i = QDateTime::currentDateTime();
    ui->lcdNumber->display(i.toString("HH:mm:ss"));
}

void MainWindow::showDisCon()
{
    qDebug()<<"ss";
    disconnectDialog a;//disconnectDialog();
    a.exec();
    qDebug()<<"ss";
}

void MainWindow::getCost(double cost)
{
    ui->payNumLabel->setText(QString::number(cost,'f',2) + "\345\205\203");//元
}

void MainWindow::getIsWork(int num)
{
    if (0 == num)
    {
        ui->label_5->setText("已被挂起");
    }
    else
    {
        ui->label_5->setText("正在服务");
    }
}
