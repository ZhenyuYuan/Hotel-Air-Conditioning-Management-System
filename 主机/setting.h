#ifndef SETTING_H
#define SETTING_H

#include <QDialog>
#include "format.h"
#include "mainframe.h"

namespace Ui {
class Setting;
}

class Setting : public QDialog
{
    Q_OBJECT

public:
    explicit Setting(QWidget *parent = 0);
    ~Setting();
    int isCold=false;
    mainframe * temM;

signals:
    void touchCold();
    void touchHot();
    void pubSig();
    //void pubSig2(bool mode,double upper,double lower,int default1);
    void trsSet(mainFrameParm*);
    void recvMainSetting(int,double,double,double,double);
    void sendSetToMainWin(int,double,double);

private slots:

    void on_buttonHot_clicked();

    void on_buttonCold_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();


private:
    Ui::Setting *ui;

};



#endif // SETTING_H
