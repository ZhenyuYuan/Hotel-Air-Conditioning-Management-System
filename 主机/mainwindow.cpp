#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "format.h"
#include "setting.h"
#include"slavelist.h"
#include"slave.h"
#include <QString>
#include <QMessageBox>
#include <QVector>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtSql>
#include <QSqlError>
#include "statementdialog.h"
#include "reportdialog.h"

extern watchToUI slaveToUI;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //QApplication a(argc,argv);
    // QDesktopWidget * desktop=QApplication::desktop();
    Setting * setDia = new Setting(this);
    setDia->setFixedSize(setDia->width(),setDia->height());
    ui->setupUi(this);
    slavelist = new slaveList();

    setDia->show();
    //setDia->move((desktop->width-setDia->width())/2,(QApplication::desktop().height()-setDia->width())/2);


    //初始定义数据库
    QFileInfo dbFileInfo("mydatabase.db");
    bool isExist = dbFileInfo.exists();
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("mydatabase.db");
    createConnection(isExist);

    connect(slavelist,SIGNAL(sendSlaveToUI(Slave*)),this,SLOT(transStru(Slave*)));
    connect(this,SIGNAL(sendPower()),slavelist,SLOT(setWorking()));
    //connect(setDia,SIGNAL(pubSig2(bool,double,double,int)),slavelist->server,SLOT(slotsendInitOrder9(bool,double,double,int)));//设置信息发给谁
    connect(setDia,SIGNAL(recvMainSetting(int,double,double,double,double)),slavelist,SLOT(recvSet(int,double,double,double,double)));
    connect(setDia,SIGNAL(sendSetToMainWin(int,double,double)),this,SLOT(mainRecvSetFunc(int,double,double)));
    this->setFixedSize(this->width(),this->height());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_toolButtonPower_clicked()
{

    if(!powerOn)
    {
        ui->toolButtonPower->setText("关机");
        powerOn=true;
        emit sendPower();

    }
    else
    {
        powerOn=false;
        ui->toolButtonPower->setText("开机");
        ui->labelModel->setText("--");
        ui->labelTemRange->setText("--");
    }
}

void MainWindow :: transStru(Slave* recS)
{
    if(recS->getno().compare("311c")==0)
    {
        if(recS->getisStart()==0)
        {
            this->mac1=0;
            ui->stateLabel_1->setText("关机");
        }
        else if(recS->getisWorking()==1)
        {
            this->mac1=1;
            ui->stateLabel_1->setText("正被处理");
        }
        else
        {
            this->mac1=1;
            ui->stateLabel_1->setText("被挂起");
        }
        ui->showIDLabel_1->setText(recS->getCustomerID());
        ui->speedLabel_1->setText(QString::number(recS->getwind()));
        ui->curTem_1->setText(QString::number(recS->getcurrentTemperature()));
        ui->tarTem_1->setText(QString::number(recS->gettargetTemperature()));
        ui->labelCost_1->setText(QString::number(recS->getTotalCost()));
    }
    if(recS->getno().compare("312c")==0)
    {
        if(recS->getisStart()==0)
        {
            this->mac2=0;
            ui->stateLabel_2->setText("关机");
        }
        else if(recS->getisWorking()==1)
        {
            this->mac2=1;
            ui->stateLabel_2->setText("正被处理");
        }
        else
        {
            this->mac2=1;
            ui->stateLabel_2->setText("被挂起");
        }
        //ui->stateLabel_2->setText(recS->getno());
        ui->showIDLabel_2->setText(recS->getCustomerID());
        ui->speedLabel_2->setText(QString::number(recS->getwind()));
        ui->curTem_2->setText(QString::number(recS->getcurrentTemperature()));
        ui->tarTem_2->setText(QString::number(recS->gettargetTemperature()));
        ui->labelCost_2->setText(QString::number(recS->getTotalCost()));
    }
    if(recS->getno().compare("313c")==0)
    {
        if(recS->getisStart()==0)
        {
            this->mac3=0;
            ui->stateLabel_3->setText("关机");
        }
        else if(recS->getisWorking()==1)
        {
            this->mac3=1;
            ui->stateLabel_3->setText("正被处理");
        }
        else
        {
            this->mac3=1;
            ui->stateLabel_3->setText("被挂起");
        }
        //ui->stateLabel_3->setText(recS->getno());
        ui->showIDLabel_3->setText(recS->getCustomerID());
        ui->speedLabel_3->setText(QString::number(recS->getwind()));
        ui->curTem_3->setText(QString::number(recS->getcurrentTemperature()));
        ui->tarTem_3->setText(QString::number(recS->gettargetTemperature()));
        ui->labelCost_3->setText(QString::number(recS->getTotalCost()));
    }
    if(recS->getno().compare("315c")==0)
    {
        if(recS->getisStart()==0)
        {
            this->mac4=0;
            ui->stateLabel_4->setText("关机");
        }
        else if(recS->getisWorking()==1)
        {
            this->mac4=1;
            ui->stateLabel_4->setText("正被处理");
        }
        else
        {
            this->mac4=1;
            ui->stateLabel_4->setText("被挂起");
        }
        //ui->stateLabel_4->setText(recS->getno());
        ui->showIDLabel_4->setText(recS->getCustomerID());
        ui->speedLabel_4->setText(QString::number(recS->getwind()));
        ui->curTem_4->setText(QString::number(recS->getcurrentTemperature()));
        ui->tarTem_4->setText(QString::number(recS->gettargetTemperature()));
        ui->labelCost_4->setText(QString::number(recS->getTotalCost()));
    }
}


void MainWindow::mainRecvSetFunc(int mode,double low,double high)
{
    if(mode==0)
        ui->labelModel->setText("制冷模式");
    else
        ui->labelModel->setText("制热模式");
    ui->labelTemRange->setText(QString::number(low)+"到"+QString::number(high));
}



void MainWindow::on_ButtonDetail_1_clicked()//显示详单按钮
{
    if(mac1==1)
       QMessageBox::information(this,"提示","该从机未关机");
    else
    {
        QVector<record> bill = slavelist->slavelist->at(0)->createBill();
        //qDebug()<<bill.count()<<"hxxqqqqqq";
        statementDialog * mainStatementDialog=new statementDialog(bill);
        mainStatementDialog->exec();
    }


}



void MainWindow::on_ButtonDetail_2_clicked()
{
    if(mac2==1)
       QMessageBox::information(this,"提示","该从机未关机");
    else
    {
        QVector<record> bill = slavelist->slavelist->at(1)->createBill();
        statementDialog * mainStatementDialog=new statementDialog(bill);
        mainStatementDialog->exec();
    }
}

void MainWindow::on_ButtonDetail_3_clicked()
{
    if(mac3==1)
       QMessageBox::information(this,"提示","该从机未关机");
    else
    {
        QVector<record> bill = slavelist->slavelist->at(2)->createBill();
        qDebug()<<bill.size()<<"xxqqqqqqhhhhh";
        statementDialog * mainStatementDialog=new statementDialog(bill);
        mainStatementDialog->exec();
    }
}

void MainWindow::on_ButtonDetail_4_clicked()
{
    if(mac4==1)
       QMessageBox::information(this,"提示","该从机未关机");
    else
    {
        QVector<record> bill = slavelist->slavelist->at(3)->createBill();
        statementDialog * mainStatementDialog=new statementDialog(bill);
        mainStatementDialog->exec();
    }
}

void MainWindow::on_toolButtonList_clicked()
{
    reportDialog * dialog = new reportDialog();
        dialog->show();
}
