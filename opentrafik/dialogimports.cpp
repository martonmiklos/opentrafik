#include "dialogimports.h"
#include "ui_dialogimports.h"
#include <QMessageBox>
#include <QDebug>

DialogImports::DialogImports(Common *common, QWidget *parent) :
    QDialog(parent, Qt::Window),
    ui(new Ui::DialogImports),
    m_common(common)
{
    ui->setupUi(this);
    m_model = new QSqlQueryModel(this);
    ui->tableViewImports->setModel(m_model);
    ui->tableViewImports->setSelectionMode(QTableView::MultiSelection);
    ui->tableViewImports->setSelectionBehavior(QTableView::SelectRows);
}

DialogImports::~DialogImports()
{
    delete ui;
}

void DialogImports::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    reloadModel();
}

void DialogImports::databaseConnected()
{
    reloadModel();
}

void DialogImports::reloadModel()
{
    m_model->setQuery("SELECT WarehouseChange.date, SUM(WarehouseChange.amount_change) as warehouse_change, SUM(WarehouseChange.amount_change*WarehouseChange.price_brutto_out) as warehouse_price_change "
                      "FROM warehouse_changes AS WarehouseChange "
                      "WHERE WarehouseChange.amount_change < 0 "
                      "GROUP BY WarehouseChange.date "
                      "ORDER BY WarehouseChange.date DESC "
                      "LIMIT 7");
    ui->tableViewImports->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableViewImports->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableViewImports->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
}

void DialogImports::on_tableViewImports_doubleClicked(const QModelIndex &index)
{

}

void DialogImports::on_pushButtonImport_clicked()
{
    m_common->importDetails()->show();
}

void DialogImports::on_pushButtonSubstracImports_clicked()
{
    QModelIndexList selection = ui->tableViewImports->selectionModel()->selectedRows();

    if (selection.size() == 2) {
        if (m_common->sqlHelper()->substractImports(
                    m_model->data(m_model->index(selection.at(0).row(), 0)).toString(),
                    m_model->data(m_model->index(selection.at(1).row(), 0)).toString()
                    )) {
            QMessageBox::information(this, tr("Substracted"), tr("Items of the import\n %1 had been removed from the\n %2 import")
                                     .arg(m_model->data(m_model->index(selection.at(1).row(), 0)).toString())
                                     .arg(m_model->data(m_model->index(selection.at(0).row(), 0)).toString()));
        }
    } else  {
        QMessageBox::information(this, tr("Select two imports"), tr("To substract imports select 2 imports please!"));
    }
}
