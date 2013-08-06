#include "dialogwarehouseitemhistory.h"
#include "ui_dialogwarehouseitemhistory.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <qmath.h>

DialogWarehouseItemHistory::DialogWarehouseItemHistory(Common *common, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogWarehouseItemHistory),
    m_common(common)
{
    m_currentId = 0;
    ui->setupUi(this);
    m_model = new QSqlQueryModel(this);
    ui->tableViewHistory->setModel(m_model);
    loadSettings();
    reloadModel();
}

DialogWarehouseItemHistory::~DialogWarehouseItemHistory()
{
    m_settings.beginGroup("gui/itemhistorydialog");
    m_settings.setValue("historytype", ui->comboBoxHistoryType->currentIndex());
    m_settings.setValue("pagesize", ui->spinBoxPageSize->value());
    m_settings.endGroup();
    delete ui;
}

void DialogWarehouseItemHistory::showHistoryOfWarehouseItem(int warehouseItemId)
{
    m_currentId = warehouseItemId;

    WarehouseItemData data = m_common->sqlHelper()->warehouseItemDataFromId(warehouseItemId);
    this->setWindowTitle(tr("Warehouse history of - %1").arg(data.name));
    ui->spinBoxPage->setValue(1);
    calcResoultCount();
    reloadModel();
    ui->labelOnStock->setText(QString::number(m_common->sqlHelper()->getItemChangeSum(warehouseItemId)));
    show();
}

void DialogWarehouseItemHistory::loadSettings()
{
    m_settings.beginGroup("gui/itemhistorydialog");
    ui->comboBoxHistoryType->setCurrentIndex(m_settings.value("historytype", 0).toInt());
    on_comboBoxHistoryType_activated(ui->comboBoxHistoryType->currentIndex());
    ui->spinBoxPageSize->setValue(m_settings.value("pagesize", 20).toInt());
    m_settings.endGroup();
}

void DialogWarehouseItemHistory::reloadModel()
{
    if (m_currentId == 0)
        return;
    QSqlQuery m_query;
    m_query.prepare("SELECT `User`.`real_name`, "
                    "`WarehouseChange`.`amount_change`, "
                    "`WarehouseChange`.`date` "
                    " FROM `warehouse_changes` AS `WarehouseChange` "
                    "LEFT JOIN `warehouse_items` AS `WarehouseItem` "
                    "ON (`WarehouseChange`.`warehouse_item_id` = `WarehouseItem`.`id`) "
                    "LEFT JOIN `users` AS `User` "
                    "ON (`WarehouseChange`.`user_id` = `User`.`id`) "
                    "WHERE `WarehouseChange`.`warehouse_item_id` = :id "+m_condition+" LIMIT :start, :pagesize");
    m_query.bindValue(":id", m_currentId);
    m_query.bindValue(":start", (ui->spinBoxPage->value()-1) * ui->spinBoxPageSize->value());
    m_query.bindValue(":pagesize", ui->spinBoxPageSize->value());
    m_query.exec();
    m_model->setQuery(m_query);
}

void DialogWarehouseItemHistory::calcResoultCount()
{
    QSqlQuery m_query;
    if (m_currentId == 0)
        return;

    m_resultCount = 0;    
    m_query.prepare("SELECT COUNT(*) AS `count` FROM `warehouse_changes` AS `WarehouseChange` WHERE `WarehouseChange`.`warehouse_item_id` = :id "+m_condition);
    m_query.bindValue(":id", m_currentId);
    if (m_query.exec()) {
        if (m_query.next()) {
            m_resultCount = m_query.value(0).toLongLong();
            ui->labelTotalResults->setText(tr("(Total %1 results)").arg(m_resultCount));
            ui->spinBoxPage->setMaximum(ceil((double)m_resultCount/(double)ui->spinBoxPageSize->value()));
            ui->spinBoxPage->setSuffix("/"+QString::number(ui->spinBoxPage->maximum()));
        }
    }
    setPageButtonsState();
}

void DialogWarehouseItemHistory::setPageButtonsState()
{
    ui->pushButtonPrev->setEnabled(ui->spinBoxPage->value() != 1);
    ui->pushButtonNext->setEnabled(ui->spinBoxPage->value() != ui->spinBoxPage->maximum());
}

void DialogWarehouseItemHistory::on_comboBoxHistoryType_activated(int )
{
    m_condition = "";
    if (ui->comboBoxHistoryType->currentIndex() == 1) {
        m_condition = "AND `WarehouseChange`.`amount_change` < 0";
    } else if (ui->comboBoxHistoryType->currentIndex() == 2) {
        m_condition = "AND `WarehouseChange`.`amount_change` > 0";
    }
    calcResoultCount();
    reloadModel();
}

void DialogWarehouseItemHistory::on_spinBoxPage_valueChanged(int )
{
    setPageButtonsState();
    reloadModel();
}

void DialogWarehouseItemHistory::on_spinBoxPageSize_valueChanged(int )
{
    calcResoultCount();
    reloadModel();
}

void DialogWarehouseItemHistory::on_pushButtonNext_clicked()
{
    ui->spinBoxPage->setValue(ui->spinBoxPage->value()+1);
}

void DialogWarehouseItemHistory::on_pushButtonPrev_clicked()
{
    ui->spinBoxPage->setValue(ui->spinBoxPage->value()-1);
}
