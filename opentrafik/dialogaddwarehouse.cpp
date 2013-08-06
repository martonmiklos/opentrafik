#include "dialogaddwarehouse.h"
#include "ui_dialogaddwarehouse.h"

#include <QDebug>
#include <QKeyEvent>
#include <QTimer>
#include <QSqlQuery>
#include <QSqlError>

DialogAddWarehouse::DialogAddWarehouse(Common *common, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddWarehouse),
    m_currentAmount(1),
    m_common(common)
{
    ui->setupUi(this);
    ui->barcodeLineEdit->setFocus();
}

DialogAddWarehouse::~DialogAddWarehouse()
{
    delete ui;
}

void DialogAddWarehouse::keyPressEvent(QKeyEvent *event)
{
    if ((event->key() == Qt::Key_Enter) || (event->key() == Qt::Key_Return)) {
        if (ui->amountDoubleSpinBox->hasFocus()) {
        }
    }
}

void DialogAddWarehouse::on_barcodeLineEdit_returnPressed()
{
    WarehouseItemData data = m_common->sqlHelper()->warehouseItemDataFromBarcode(ui->barcodeLineEdit->text());
    if (data.id == 0) {
        ui->barcodeLineEdit->setFocus();
        ui->labelMessage->setStyleSheet("background-color: red;");
        ui->labelMessage->setText(tr("The %1 barcode does not belongs to any items!").arg(ui->barcodeLineEdit->text()));
    } else {
        if (data.amount != 1) {
            ui->labelMessage->setText(tr("%1 pcs of %2").arg(data.amount).arg(data.name));
        } else {
            ui->labelMessage->setText(data.name);
        }

        ui->amountDoubleSpinBox->selectAll();
        ui->amountDoubleSpinBox->setFocus();
        ui->labelMessage->setStyleSheet("background-color: #15cd10;");
        QTimer::singleShot(1000, this, SLOT(removeGreen()));
    }
}

void DialogAddWarehouse::on_amountDoubleSpinBox_editingFinished()
{
    ui->pushButtonAdd->setFocus();
}

void DialogAddWarehouse::on_pushButtonAdd_clicked()
{
    bool added = false;
    QString message = tr("The %1 barcode does not belongs to any items!").arg(ui->barcodeLineEdit->text());
    WarehouseItemData data = m_common->sqlHelper()->warehouseItemDataFromBarcode(ui->barcodeLineEdit->text());
    double amount = ui->amountDoubleSpinBox->value()*data.amount;
    if (data.id > 0) {
        QSqlQuery query;
        query.prepare("UPDATE warehouse_items SET amount = amount + :amount WHERE id = :warehouse_item_id");
        query.bindValue(":amount", amount);
        query.bindValue(":warehouse_item_id", data.id);
        if (query.exec()) {
            added = true;
            m_common->sqlHelper()->saveAmountChange(data.id, amount, data.bruttoSellPrice);
        } else {
            qWarning() << query.lastError().text();
        }
    }

    ui->labelMessage->setText(message);
    ui->barcodeLineEdit->setFocus();
    if (added) {
        ui->barcodeLineEdit->clear();
        ui->labelMessage->setText(tr("%1 pcs. of %2 added")
                                  .arg(amount)
                                  .arg(data.name));
        ui->amountDoubleSpinBox->setValue(1);
        m_currentItemName = "";
        m_currentAmount = 1;
    } else {
        ui->barcodeLineEdit->selectAll();
    }
    QTimer::singleShot(3000, this, SLOT(clearMessage()));
}


void DialogAddWarehouse::clearMessage()
{
    ui->labelMessage->clear();
}

void DialogAddWarehouse::removeGreen()
{
    ui->labelMessage->setStyleSheet("");
}
