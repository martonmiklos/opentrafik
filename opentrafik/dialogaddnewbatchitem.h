#ifndef DIALOGADDNEWBATCHITEM_H
#define DIALOGADDNEWBATCHITEM_H

#include <QDialog>
#include "common.h"

namespace Ui {
class DialogAddNewBatchItem;
}

class DialogAddNewBatchItem : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogAddNewBatchItem(Common *common, QWidget *parent = 0);
    ~DialogAddNewBatchItem();
    void addNewBatchItemForItem(int warehouseItemId, QString wareHouseItemName);
    
private slots:
    void on_pushButtonAdd_clicked();


private:
    Ui::DialogAddNewBatchItem *ui;
    int m_warehouseItemId;
    Common *m_common;
};

#endif // DIALOGADDNEWBATCHITEM_H
