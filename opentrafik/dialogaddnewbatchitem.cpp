#include "dialogaddnewbatchitem.h"
#include "ui_dialogaddnewbatchitem.h"

#include <QDebug>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>

DialogAddNewBatchItem::DialogAddNewBatchItem(Common *common, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddNewBatchItem),
    m_common(common)
{
    ui->setupUi(this);
}

DialogAddNewBatchItem::~DialogAddNewBatchItem()
{
    delete ui;
}

void DialogAddNewBatchItem::addNewBatchItemForItem(int warehouseItemId, QString wareHouseItemName)
{
    ui->barcodeLineEdit->clear();
    ui->descriptionLineEdit->clear();
    ui->batchQuantityDoubleSpinBox->setValue(0);
    this->setWindowTitle(tr("Add new batch item to: %1").arg(wareHouseItemName));
    m_warehouseItemId = warehouseItemId;
    this->show();
    ui->barcodeLineEdit->setFocus();
}

void DialogAddNewBatchItem::on_pushButtonAdd_clicked()
{
    if (m_common->sqlHelper()->barcodeExists(ui->barcodeLineEdit->text())) {
        QMessageBox::warning(this,
                             tr("Conflict"),
                             tr("Batch item with barcode %1 already exists").arg(ui->barcodeLineEdit->text()));
        ui->barcodeLineEdit->setFocus();
        return;
    } else if (ui->batchQuantityDoubleSpinBox->value() == 0) {
        QMessageBox::warning(this,
                             tr("Error"),
                             tr("Batch quantity cannot be zero"));
        ui->batchQuantityDoubleSpinBox->setFocus();
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT into warehouse_batch_items (barcode, description, warehouse_item_id, amount) "
                  "values(:barcode, :description, :warehouse_item_id, :amount)");
    query.bindValue(":barcode", ui->barcodeLineEdit->text());
    query.bindValue(":warehouse_item_id", m_warehouseItemId);
    query.bindValue(":description", ui->descriptionLineEdit->text());
    query.bindValue(":amount", ui->batchQuantityDoubleSpinBox->value());
    if (!query.exec()) {
        qDebug() << query.lastQuery() << query.lastError().text();
    } else {
        this->close();
    }
}
