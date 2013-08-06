#include "dialogadvancedwarehousefilter.h"
#include "ui_dialogadvancedwarehousefilter.h"

DialogAdvancedWarehouseFilter::DialogAdvancedWarehouseFilter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAdvancedWarehouseFilter)
{
    ui->setupUi(this);
}

DialogAdvancedWarehouseFilter::~DialogAdvancedWarehouseFilter()
{
    delete ui;
}

void DialogAdvancedWarehouseFilter::on_checkBox_toggled(bool checked)
{
    if (checked) {
        emit filterChanged(" price_brutto_in = 0 ");
    } else {
        emit filterChanged("");
    }
}
