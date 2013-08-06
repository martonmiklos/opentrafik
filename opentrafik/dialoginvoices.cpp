#include "dialoginvoices.h"
#include "ui_dialoginvoices.h"

DialogInvoices::DialogInvoices(Common *common, QWidget *parent) :
    DialogBaseWithModel(common, parent),
    ui(new Ui::DialogInvoices)
{
    ui->setupUi(this);
    ui->tableView->setModel(m_model);
    ui->tableView->setSelectionMode(QTableView::SingleSelection);
    ui->tableView->setSelectionBehavior(QTableView::SelectRows);

    addInvoiceDialog = new DialogAddInvoice(this);
}

DialogInvoices::~DialogInvoices()
{
    delete ui;
}

void DialogInvoices::showEvent(QShowEvent *)
{
    on_comboBoxSupplier_currentIndexChanged(ui->comboBoxSupplier->currentIndex());
}

void DialogInvoices::showSuppliersInvoices(quint32 supplierId)
{

}

void DialogInvoices::databaseConnected()
{
    ui->comboBoxSupplier->reload();
}

void DialogInvoices::reloadModel()
{
    QString query = "SELECT Invoice.id, Invoice.invoice_number, Invoice.item_count, Invoice.sum_brutto, SUM(WI.amount*brutto_buy_price) "
            "FROM invoices AS Invoice "
            "LEFT JOIN warehouse_items_stock AS WI "
            "ON WI.invoice_id = Invoice.id";
    if (!m_supplierFilter.isEmpty() || !m_completeFilter.isEmpty()) {
        query.append(" WHERE ");
        query.append(m_supplierFilter); // FIXME
    }
    m_model->setQuery(query);
    /*m_model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    m_model->setHeaderData(1, Qt::Horizontal, tr("Invoice number"));
    m_model->setHeaderData(2, Qt::Horizontal, tr("Item count"));
    m_model->setHeaderData(3, Qt::Horizontal, tr("Brutto price"));
    m_model->setHeaderData(4, Qt::Horizontal, tr("Booked brutto price"));

    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);*/


}

void DialogInvoices::on_comboBoxSupplier_currentIndexChanged(int index)
{
    if (index == 0) {
        m_supplierFilter = "";
    } else {
        m_supplierFilter = QString("Invoice.supplier_id = %1").arg(ui->comboBoxSupplier->itemData(index).toLongLong());
    }
    reloadModel();
}

void DialogInvoices::on_checkBoxIncomplete_toggled(bool checked)
{
    if (checked) {
        m_completeFilter = ""; // FIXME
    } else {
        m_completeFilter = "";
    }
    reloadModel();
}

void DialogInvoices::on_pushButtonNew_clicked()
{
    addInvoiceDialog->addInvoiceToSupplier(ui->comboBoxSupplier->currentSupplierId());
}
