#include "dialogaddinvoice.h"
#include "ui_dialogaddinvoice.h"

DialogAddInvoice::DialogAddInvoice(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddInvoice)
{
    ui->setupUi(this);
}

DialogAddInvoice::~DialogAddInvoice()
{
    delete ui;
}

void DialogAddInvoice::addInvoiceToSupplier(qlonglong supplierId)
{
    ui->supplierLabel->setVisible(supplierId == 0);
    ui->supplierComboBox->setVisible(supplierId == 0);
    adjustSize();
    show();
}
