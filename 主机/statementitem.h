#ifndef STATEMENTITEM_H
#define STATEMENTITEM_H


#include <QWidget>
#include "format.h"

namespace Ui {
class statementItem;
}

class statementItem : public QWidget
{
    Q_OBJECT

public:
    explicit statementItem(record billItem, QWidget *parent = 0);
    ~statementItem();

private:
    Ui::statementItem *ui;
};

#endif // STATEMENTITEM_H
