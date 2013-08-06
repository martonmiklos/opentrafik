#include "dialogwarehouseitemsbatchitems.h"
#include "ui_dialogwarehouseitemsbatchitems.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <QMessageBox>

DialogWareHouseItemsBatchItems::DialogWareHouseItemsBatchItems(Common *common, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogWareHouseItemsBatchItems),
    m_common(common)
{
    ui->setupUi(this);
    m_model = new QSqlQueryModel(this);
    ui->tableView->setModel(m_model);
    ui->tableView->setSelectionMode(QTableView::SingleSelection);
    ui->tableView->setSelectionBehavior(QTableView::SelectRows);
    m_currentId = 0;
}

DialogWareHouseItemsBatchItems::~DialogWareHouseItemsBatchItems()
{
    delete ui;
}

void DialogWareHouseItemsBatchItems::showWareHouseItemsBatches(const int warehouseItemId, const QString warehouseItemName)
{
    m_warehouseItemId = warehouseItemId;
    m_currentId = 0;
    this->setWindowTitle(tr("Batches of %1").arg(warehouseItemName));
    reloadModel();
    ui->barcodeLineEdit->clear();
    ui->amountDoubleSpinBox->setValue(10);
    ui->descriptionTextEdit->clear();
    ui->barcodeLineEdit->setFocus();
    this->show();
}

void DialogWareHouseItemsBatchItems::reloadModel()
{
    QSqlQuery m_modelQuery;
    m_modelQuery.prepare("SELECT id, barcode, amount, description FROM warehouse_batch_items WHERE warehouse_item_id = :id");
    m_modelQuery.bindValue(":id", m_warehouseItemId);
    m_modelQuery.exec();
    m_model->setQuery(m_modelQuery);
    m_model->setHeaderData(1, Qt::Horizontal, tr("Barcode"), Qt::DisplayRole);
    m_model->setHeaderData(2, Qt::Horizontal, tr("Amount"), Qt::DisplayRole);
    m_model->setHeaderData(3, Qt::Horizontal, tr("Description"), Qt::DisplayRole);


#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
#else
    ui->tableView->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setResizeMode(2, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setResizeMode(3, QHeaderView::Stretch);
#endif

    ui->tableView->setColumnHidden(0, true); // hide ID
}

bool DialogWareHouseItemsBatchItems::checkValues()
{
    bool ret = true;
    QString errorString = "";
    QWidget *focusWidget = NULL;
    if (ui->barcodeLineEdit->text().isEmpty()) {
        ret = false;
        errorString = tr("The barcode cannot left empty!");
        focusWidget = ui->barcodeLineEdit;
    } else if (ui->amountDoubleSpinBox->value() == 0) {
        ret = false;
        errorString = tr("Batch with zero item amount?");
        focusWidget = ui->amountDoubleSpinBox;
    }

    if (!ret) {
        QMessageBox::warning(this, tr("Error in input data"), errorString);
        focusWidget->setFocus();
    }

    return ret;
}

void DialogWareHouseItemsBatchItems::on_pushButtonSave_clicked()
{
    if (!ui->barcodeLineEdit->text().isEmpty()) {
        QString conflictName;
        if (m_common->sqlHelper()->barcodeExists(ui->barcodeLineEdit->text(), m_currentId, NULL, &conflictName)) {
            QMessageBox::warning(this,
                                 tr("Conflict"),
                                 tr("The %1 item has this barcode already!").arg(conflictName));
            ui->barcodeLineEdit->selectAll();
            return;
        }
    }

    if (!checkValues()) {
        return;
    }


    QSqlQuery query;
    if (m_currentId > 0) {
        query.prepare("UPDATE warehouse_batch_items SET " \
                      "barcode = :barcode, " \
                      "description = :description, " \
                      "amount = :amount " \
                      "WHERE id = :id");
        query.bindValue(":id", m_currentId);
    } else {
        query.prepare("INSERT into warehouse_batch_items (barcode, description, warehouse_item_id, amount) "
                      "values(:barcode, :description, :warehouse_item_id, :amount)");
        query.bindValue(":warehouse_item_id", m_warehouseItemId);
    }

    query.bindValue(":description", ui->descriptionTextEdit->toPlainText());
    query.bindValue(":amount", ui->amountDoubleSpinBox->value());
    query.bindValue(":barcode", ui->barcodeLineEdit->text());

    if (query.exec()) {
        reloadModel();
    } else {
        qWarning() << query.lastQuery() << query.lastError().text();
    }
}

void DialogWareHouseItemsBatchItems::on_pushButtonDelete_clicked()
{
    if (QMessageBox::question(this, tr("Are you sure?"), tr("Do you really want to delete this batch item?")) == QMessageBox::No)
        return;

    if (m_currentId > 0) {
        QSqlQuery query;
        query.prepare("DELETE FROM warehouse_batch_items WHERE id = :id");
        query.bindValue(":id", m_currentId);
        if (query.exec()) {
            reloadModel();
        } else {
            qWarning() << query.lastQuery() << query.lastError().text();
        }
    }
    m_currentId = 0;
}

void DialogWareHouseItemsBatchItems::on_tableView_activated(const QModelIndex &index)
{
    m_currentId = m_model->record(index.row()).value(0).toInt();
    ui->barcodeLineEdit->setText(m_model->record(index.row()).value(1).toString());
    ui->amountDoubleSpinBox->setValue(m_model->record(index.row()).value(2).toDouble());
    ui->descriptionTextEdit->setText(m_model->record(index.row()).value(3).toString());
}

void DialogWareHouseItemsBatchItems::on_pushButtonAdNew_clicked()
{
    m_currentId = 0;
    ui->barcodeLineEdit->clear();
    ui->amountDoubleSpinBox->setValue(10);
    ui->descriptionTextEdit->clear();
}
