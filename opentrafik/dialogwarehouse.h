#ifndef DIALOGWAREHOUSE_H
#define DIALOGWAREHOUSE_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QItemSelection>
#include <QTimer>
#include <QCalendarWidget>
#include <QMap>

#include "dialogaddnewbatchitem.h"
#include "dialogwarehouseitemsbatchitems.h"
#include "dialogwarehouseitemhistory.h"
#include "common.h"
#include "warehouseitemnamecompleter.h"
#include "dialogadvancedwarehousefilter.h"

namespace Ui {
class DialogWarehouse;
}

class DialogWarehouse : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogWarehouse(Common *common, QWidget *parent = 0);
    ~DialogWarehouse();
    void databaseConnected();
    void showEvent(QShowEvent *);

public slots:
    void exportPLUFile();

private slots:
    void on_pushButtonSave_clicked();
    void on_pushButtonAddNew_clicked();
    void on_pushButtonDeleteCurrent_clicked();
    void on_pushButtonWarehouseBatchItems_clicked();
    void on_priceInDoubleSpinBox_valueChanged(double arg1);
    void on_priceOutDoubleSpinBox_valueChanged(double arg1);
    void on_taxTypeComboBox_currentIndexChanged(int index);
    void on_pushButtonHistory_clicked();
    void on_pushButtonExport_clicked();
    void on_pushButtonImport_clicked();
    void on_lineEditFilter_textChanged(const QString &arg1);
    void on_actionSearch_triggered();
    void tableViewSelectionChanged(const QItemSelection & selected, const QItemSelection & deselected);
    void clearStatusMessage();
    void dataChanged();
    void sortItems(int column, Qt::SortOrder order);
    void on_tableView_doubleClicked(const QModelIndex &index);
    void on_pushButtonAdvancedFilter_clicked();
    void advancedFilterChanged(QString filter);

private:
    Ui::DialogWarehouse *ui;
    quint32 m_currentId;
    double m_currentOnStock;
    QSqlQueryModel *m_model;
    WarehouseItemNameCompleter *m_completer;

    void reloadModel();
    void calcNetto();

    void addNewItem();
    void saveCurrentItem();
    bool checkValues();

    QString getLastExecutedQuery(const QSqlQuery& query);

    DialogAddNewBatchItem *batchItemAddDialog;
    DialogWareHouseItemsBatchItems *batchItemsDialog;
    DialogWarehouseItemHistory *historyDialog;
    DialogAdvancedWarehouseFilter *advancedFilter;

    Common *m_common;
    QSettings m_settings;
    QString m_nameFilter;
    QString m_importFileName;
    bool m_changed;

    bool writeExportFile(QString fileName);
    void setStatusLabel(QString statusText);
    QString m_sortField, m_advancedFilter;
    QMap<int, QString> m_sortFieldsMap;
};

#endif // DIALOGWAREHOUSE_H
