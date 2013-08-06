#include "dialogaddpricebruttoin.h"
#include "ui_dialogaddpricebruttoin.h"
#include <QDebug>

DialogAddPriceBruttoIn::DialogAddPriceBruttoIn(Common *common, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddPriceBruttoIn),
    m_common(common)
{
    ui->setupUi(this);
}

DialogAddPriceBruttoIn::~DialogAddPriceBruttoIn()
{
    delete ui;
}

void DialogAddPriceBruttoIn::keyPressEvent(QKeyEvent *event)
{
    if (ui->doubleSpinBoxBuyPrice->hasFocus()) {
        if ((event->key() == Qt::Key_Enter) || (event->key() == Qt::Key_Return)) {
        }
    }
}

void DialogAddPriceBruttoIn::on_lineEditBarcode_returnPressed()
{
    m_currentData = m_common->sqlHelper()->warehouseItemDataFromBarcode(ui->lineEditBarcode->text());
    if (m_currentData.id != 0) {
        this->setWindowTitle(m_currentData.name);
        ui->doubleSpinBoxBuyPrice->setValue(m_currentData.bruttoBuyPrice);
        this->setStyleSheet("background: #15cd10");
        ui->doubleSpinBoxBuyPrice->setFocus();
        ui->doubleSpinBoxBuyPrice->selectAll();
    } else {
        ui->lineEditBarcode->selectAll();
        this->setStyleSheet("background: red");
        ui->lineEditBarcode->setFocus();
    }
}

void DialogAddPriceBruttoIn::on_pushButtonSave_clicked()
{
    m_currentData.bruttoBuyPrice = ui->doubleSpinBoxBuyPrice->value();

    if (m_common->sqlHelper()->saveWareHouseItemData(m_currentData)) {
        this->setStyleSheet("background: green");
    } else {
        this->setStyleSheet("background: red");
    }

    this->setWindowTitle(tr("Add buy prices"));
    ui->doubleSpinBoxBuyPrice->setValue(0);
    ui->lineEditBarcode->selectAll();
    ui->lineEditBarcode->setFocus();
}
