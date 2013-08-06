#include "dialogcorrector.h"
#include "ui_dialogcorrector.h"

DialogCorrector::DialogCorrector(Common *common, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCorrector),
    m_common(common)
{
    ui->setupUi(this);
}

DialogCorrector::~DialogCorrector()
{
    delete ui;
}

void DialogCorrector::on_lineEdit_returnPressed()
{
    if (ui->lineEdit->text().contains('2')) {
        QString barcode = ui->lineEdit->text().replace('2', '0');
        WarehouseItemData data = m_common->sqlHelper()->warehouseItemDataFromBarcode(barcode);
        if (data.id != 0) {
            data.barcode = ui->lineEdit->text();
            if (m_common->sqlHelper()->saveWareHouseItemData(data)) {
                this->setStyleSheet("background: green");
            } else
                this->setStyleSheet("background: red");
        } else  {
            this->setStyleSheet("background: orange");
        }
        ui->lineEdit->selectAll();
        ui->lineEdit->setFocus();
    } else {
        this->setStyleSheet("background: blue");
    }
    ui->lineEdit->selectAll();
    ui->lineEdit->setFocus();
}
