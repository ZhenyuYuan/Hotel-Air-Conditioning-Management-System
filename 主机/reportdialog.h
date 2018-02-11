#ifndef REPORTDIALOG_H
#define REPORTDIALOG_H

#include <QDialog>
#include <QObject>
#include <QSqlTableModel>

namespace Ui {
class reportDialog;
}

class reportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit reportDialog(QWidget *parent = 0);
    ~reportDialog();
    QSqlTableModel *model;

private slots:
    void on_buttonOk_clicked();

    void on_roomNumComboBox_activated(const QString &arg1);

private:
    Ui::reportDialog *ui;
};

#endif // REPORTDIALOG_H
