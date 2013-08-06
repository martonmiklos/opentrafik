#include "dialogsellitem.h"
#include "ui_dialogsellitem.h"

#include <QTimer>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DialogSellItem::DialogSellItem(Common *common, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSellItem),
    m_common(common)
{
    ui->setupUi(this);
}

DialogSellItem::~DialogSellItem()
{
    delete ui;
}

void DialogSellItem::on_barcodeLineEdit_returnPressed()
{
    WarehouseItemData data = m_common->sqlHelper()->warehouseItemDataFromBarcode(ui->barcodeLineEdit->text());

    if (data.id > 0) {
        QSqlQuery query;
        query.prepare("UPDATE warehouse_items SET amount = amount - :amount WHERE id = :id");
        query.bindValue(":id", data.id);
        query.bindValue(":amount", data.amount);
        if (query.exec()) {
            if (data.amount == 1)
                ui->labelMessage->setText(tr("%1 added").arg(data.name));
            else
                ui->labelMessage->setText(tr("%1 pcs of %2 added").arg(data.amount).arg(data.name));
            ui->labelMessage->setStyleSheet("background-color: #15cd10;");
            m_common->sqlHelper()->saveAmountChange(data.id, -data.amount, data.bruttoSellPrice);
        } else {
            qWarning() << query.lastError().text();
        }

    } else {
        ui->labelMessage->setStyleSheet("background-color: red;");
        ui->labelMessage->setText(tr("The %1 barcode does not belongs to any items!").arg(ui->barcodeLineEdit->text()));
    }
    QTimer::singleShot(2000, this, SLOT(clearColor()));
    ui->barcodeLineEdit->clear();
    ui->barcodeLineEdit->setFocus();
}

void DialogSellItem::clearColor()
{
    ui->labelMessage->clear();
    ui->labelMessage->setStyleSheet("");
}
