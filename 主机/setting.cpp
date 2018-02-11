#include "setting.h"
#include "ui_setting.h"
#include "format.h"
#include "mainframe.h"
#include "mainwindow.h"
#include "tcpclientsocket.h"
#include<qdebug.h>
#include <QMessageBox>
#include <QDesktopWidget>

//mainFrameParm mainMac2;

Setting::Setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Setting)
{
    QDesktopWidget * desktop=QApplication::desktop();
    move((desktop->width()-this->width())/2,(desktop->height()-this->height())/2);
    //setWindowFlags(this->windowFlags()|Qt::FramelessWindowHint);


    ui->setupUi(this);
    ui->lineEditCost->setText("23");
    ui->lineEditDef->setText("18");
    ui->lineEditHigh->setText("26");
    ui->lineEditLow->setText("12");
    temM=new mainframe();


    if(isCold==0)
    {
        ui->buttonCold->setEnabled(false);
        ui->buttonCold->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 108, 255);\nborder-style: solid");
        ui->buttonHot->setStyleSheet("");
        ui->buttonHot->setEnabled(true);
    }
    else
    {
        ui->buttonHot->setEnabled(false);
        ui->buttonHot->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(255, 85, 0);\nborder-style: solid");
        ui->buttonCold->setStyleSheet("");
        ui->buttonCold->setEnabled(true);
    }
    connect(this,SIGNAL(touchCold()),this,SLOT(on_buttonCold_clicked()));
    connect(this,SIGNAL(touchHot()),this,SLOT(on_buttonHot_clicked()));
    connect(this,SIGNAL(pubSig()),this,SLOT(on_buttonBox_accepted()));
    connect(this,SIGNAL(pubSig()),this,SLOT(on_buttonBox_rejected()));
    connect(this,SIGNAL(trsSet(mainFrameParm*)),temM,SLOT(initMainframe(mainFrameParm*)));

    setAttribute(Qt::WA_DeleteOnClose);
}

Setting::~Setting()
{
    delete ui;
}


void Setting::on_buttonHot_clicked()
{
    ui->buttonHot->setEnabled(false);
    ui->buttonHot->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(255, 85, 0);\nborder-style: solid");
    ui->buttonCold->setStyleSheet("");
    ui->buttonCold->setEnabled(true);
    isCold = 1;
    qDebug()<<"设为制热模式";
}

void Setting::on_buttonCold_clicked()
{
    ui->buttonCold->setEnabled(false);
    ui->buttonCold->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 108, 255);\nborder-style: solid");
    ui->buttonHot->setStyleSheet("");
    ui->buttonHot->setEnabled(true);
    isCold = 0;
    qDebug()<<"设为制冷模式";
}

void Setting::on_buttonBox_accepted()
{
    mainFrameParm* temFrame=new mainFrameParm();

    if(ui->lineEditCost->text().isEmpty()||ui->lineEditHigh->text().isEmpty()||ui->lineEditLow->text().isEmpty()||ui->lineEditDef->text().isEmpty())
    {
        QMessageBox::warning(NULL, "提示","设置不能为空。", QMessageBox::Yes | QMessageBox::Yes);
    }
    //temFrame->isWorking=1;
    temFrame->workmode=this->isCold;
    temFrame->costPerW=ui->lineEditCost->text().toDouble();
    temFrame->defTemperature=ui->lineEditDef->text().toDouble();
    temFrame->maxTemperature=ui->lineEditHigh->text().toDouble();
    temFrame->minTemperature=ui->lineEditLow->text().toDouble();

    emit trsSet(temFrame);
    //bool mode=this->isCold;
    //double upper=ui->lineEditHigh->text().toDouble();
    //double lower=ui->lineEditLow->text().toDouble();
    //int default1=ui->lineEditDef->text().toDouble();
    emit recvMainSetting(temFrame->workmode,temFrame->minTemperature,temFrame->maxTemperature,temFrame->defTemperature,temFrame->costPerW);
    //emit pubSig2(mode,upper,lower,default1);


    emit sendSetToMainWin(this->isCold,ui->lineEditLow->text().toDouble(),ui->lineEditHigh->text().toDouble());


    qDebug()<<"成功关闭";
    this->done(0);
}

void Setting::on_buttonBox_rejected()
{
    qDebug()<<"QUIT";
    this->done(0);
}
