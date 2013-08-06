#include "dialogitemdetails.h"
#include "ui_dialogitemdetails.h"
#include <QShowEvent>

DialogItemDetails::DialogItemDetails(Common *common, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogItemDetails),
    m_common(common)
{
    ui->setupUi(this);
}

DialogItemDetails::~DialogItemDetails()
{
    delete ui;
}

void DialogItemDetails::showEvent(QShowEvent *event)
{
    event->accept();
    clearData();
}

void DialogItemDetails::on_lineEditBarcode_returnPressed()
{
    WarehouseItemData data = m_common->sqlHelper()->warehouseItemDataFromBarcode(ui->lineEditBarcode->text());
    if (data.id != 0) {
        ui->labelName->setText(data.name);
        ui->labelOnStock->setText(QString("%1").arg(data.onStock));
        ui->labelBruttoSellPrice->setText(QString("%1").arg(data.bruttoSellPrice));
        ui->labelBruttoInPrice->setText(QString("%1").arg(data.bruttoBuyPrice));
        ui->labelDescription->setText(data.description);
        ui->labelTaxType->setText(m_common->sqlHelper()->taxNameFromTaxId(data.taxId));
        ui->labelStatus->clear();
    } else {
        clearData();
        ui->labelStatus->setText(tr("Item with barcode: %1 not found").arg(ui->lineEditBarcode->text()));
    }
    ui->lineEditBarcode->setFocus();
    ui->lineEditBarcode->selectAll();
}


void DialogItemDetails::clearData()
{
    ui->labelName->clear();
    ui->labelBruttoInPrice->clear();
    ui->labelBruttoSellPrice->clear();
    ui->labelDescription->clear();
    ui->labelName->clear();
    ui->labelOnStock->clear();
    ui->labelStatus->clear();
    ui->labelTaxType->clear();
}
