#ifndef DIALOGWAREHOUSEITEMHISTORY_H
#define DIALOGWAREHOUSEITEMHISTORY_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QSettings>

#include "common.h"

namespace Ui {
class DialogWarehouseItemHistory;
}

class DialogWarehouseItemHistory : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogWarehouseItemHistory(Common *common, QWidget *parent = 0);
    ~DialogWarehouseItemHistory();
    void showHistoryOfWarehouseItem(int warehouseItemId);
private slots:
    void on_comboBoxHistoryType_activated(int index);

    void on_spinBoxPage_valueChanged(int arg1);

    void on_spinBoxPageSize_valueChanged(int arg1);

    void on_pushButtonNext_clicked();

    void on_pushButtonPrev_clicked();

private:
    Ui::DialogWarehouseItemHistory *ui;
    QSqlQueryModel *m_model;
    QSettings m_settings;
    int m_currentId;
    qlonglong m_resultCount;
    QString m_condition;
    Common *m_common;

    void loadSettings();
    void reloadModel();
    void calcResoultCount();
    void setPageButtonsState();
};

#endif // DIALOGWAREHOUSEITEMHISTORY_H
