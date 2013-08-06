#ifndef DIALOGWAREHOUSEITEMSBATCHITEMS_H
#define DIALOGWAREHOUSEITEMSBATCHITEMS_H

#include <QDialog>
#include <QSqlQueryModel>

#include "common.h"

namespace Ui {
class DialogWareHouseItemsBatchItems;
}

class DialogWareHouseItemsBatchItems : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogWareHouseItemsBatchItems(Common *common, QWidget *parent = 0);
    ~DialogWareHouseItemsBatchItems();
    void showWareHouseItemsBatches(const int warehouseItemId, const QString warehouseItemName);
    
private slots:
    void on_pushButtonSave_clicked();

    void on_pushButtonDelete_clicked();

    void on_tableView_activated(const QModelIndex &index);

    void on_pushButtonAdNew_clicked();

private:
    Ui::DialogWareHouseItemsBatchItems *ui;
    QSqlQueryModel *m_model;
    int m_currentId;
    int m_warehouseItemId;

    void reloadModel();
    bool checkValues();

    Common *m_common;
};

#endif // DIALOGWAREHOUSEITEMSBATCHITEMS_H
