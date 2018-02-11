#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTcpSocket>
#include <QTcpServer>
#include <QVector>
#include <QSqlDatabase>
#include <QFileInfo>

#include "format.h"
#include "setting.h"
#include "slavelist.h"


namespace Ui {
class MainWindow;
}



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    slaveList * slavelist;

    //mainFrameParm* temFrame;

    bool isBinded=false;
    bool isSet=false;
    bool powerOn=false;


    int mac1=0;
    int mac2=0;
    int mac3=0;
    int mac4=0;

    int mainWindowMode;
    double mainTemHigh;
    double mainTemLow;

    QSqlDatabase db;

    void createConnection(bool isExist);


signals:
    void receiveSlaveToUI(Slave*);
    void sendPower();
    //void pubSig();



private slots:
    void on_toolButtonPower_clicked();

    //void on_toolButtonSetting_clicked();

    void transStru(Slave*);

    void mainRecvSetFunc(int,double,double);

    //void mainRecvID(QString);

    void on_ButtonDetail_1_clicked();

    void on_ButtonDetail_2_clicked();

    void on_ButtonDetail_3_clicked();

    void on_ButtonDetail_4_clicked();

    void on_toolButtonList_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

