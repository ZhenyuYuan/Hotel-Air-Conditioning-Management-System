#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "slave.h"
#include "logindialog.h"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void send()
        {emit touchSwitch();}
    void sendTarTem(int tem)
        {emit tarTem(tem);}
    void sendWindSpeed(int wind)
        {emit windSpeed(wind);}
    ~MainWindow();
    Slave *slave;
signals:
    void touchSwitch();
    void tarTem(int);
    void windSpeed(int);

private slots:

    void on_switchButton_clicked();
    void test(int);
    void getData(double);

    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

    void getWorkMode(const int workMode,double upper,double lower, double default1, int wind);

    void on_toolButton_3_clicked();

    void on_toolButton_4_clicked();

    void onTimeOut();

    void showDisCon();

    void getCost(double);

    void getIsWork(int);

private:
    Ui::MainWindow *ui;
    loginDialog *login;
    int wind;
    int workMode;
    double upper;
    double lower;
    int tem;
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
