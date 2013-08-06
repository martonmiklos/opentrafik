#include "dialogwarehouse.h"
#include "ui_dialogwarehouse.h"

#include <QMessageBox>
#include <QHeaderView>
#include <QDebug>
#include <QShowEvent>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlQuery>
#include <QFileDialog>
#include <QFileInfo>
#include <QAction>

DialogWarehouse::DialogWarehouse(Common *common, QWidget *parent) :
    QDialog(parent, Qt::Window),
    ui(new Ui::DialogWarehouse),
    m_common(common),
    m_changed(false)
{
    ui->setupUi(this);
    m_currentId = 0;
    m_model = NULL;
    m_completer = NULL;
    m_sortField = "id";

    m_sortFieldsMap[0] = "id";
    m_sortFieldsMap[1] = "name";
    m_sortFieldsMap[4] = "price_brutto_out";
    m_sortFieldsMap[7] = "amount";

    QAction *searchAction = new QAction(this);
    searchAction->setShortcut(QKeySequence("Ctrl+F"));
    this->addAction(searchAction);
    connect(searchAction, SIGNAL(triggered()), this, SLOT(on_actionSearch_triggered()));
    connect(ui->nameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(dataChanged()));
    connect(ui->descriptionTextEdit, SIGNAL(textChanged()), this, SLOT(dataChanged()));
    connect(ui->barcodeLineEdit, SIGNAL(textChanged(QString)), this, SLOT(dataChanged()));
    connect(ui->priceInDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(dataChanged()));
    connect(ui->priceOutDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(dataChanged()));
    connect(ui->taxTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(dataChanged()));
    connect(ui->onStockDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(dataChanged()));

    connect(ui->tableView->horizontalHeader(), SIGNAL(sortIndicatorChanged(int,Qt::SortOrder)),
            this, SLOT(sortItems(int,Qt::SortOrder)));

    advancedFilter = new DialogAdvancedWarehouseFilter(this);
    connect(advancedFilter, SIGNAL(filterChanged(QString)), this, SLOT(advancedFilterChanged(QString)));
}

void DialogWarehouse::databaseConnected()
{
    m_model = new QSqlQueryModel(this);
    ui->tableView->setModel(m_model);
    connect(ui->tableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(tableViewSelectionChanged(QItemSelection,QItemSelection)));

    ui->taxTypeComboBox->reload();
    reloadModel();

    m_completer = new WarehouseItemNameCompleter(this);
    ui->nameLineEdit->setCompleter(m_completer);

    m_model->setHeaderData(0, Qt::Horizontal, tr("ID"), Qt::DisplayRole);
    m_model->setHeaderData(1, Qt::Horizontal, tr("Name"), Qt::DisplayRole);
    m_model->setHeaderData(4, Qt::Horizontal, tr("Brutto sell price"), Qt::DisplayRole);
    m_model->setHeaderData(7, Qt::Horizontal, tr("Amount"), Qt::DisplayRole);
    ui->tableView->setColumnHidden(2, true);
    ui->tableView->setColumnHidden(3, true);
    ui->tableView->setColumnHidden(5, true);
    ui->tableView->setColumnHidden(6, true);

    ui->tableView->setSelectionMode(QTableView::SingleSelection);
    ui->tableView->setSelectionBehavior(QTableView::SelectRows);

    batchItemAddDialog = new DialogAddNewBatchItem(m_common, this);
    batchItemsDialog = new DialogWareHouseItemsBatchItems(m_common, this);
    historyDialog = new DialogWarehouseItemHistory(m_common, this);
}

DialogWarehouse::~DialogWarehouse()
{
    delete ui;
}

void DialogWarehouse::showEvent(QShowEvent *event)
{
    reloadModel();
    event->accept();
}

void DialogWarehouse::exportPLUFile()
{
    m_settings.beginGroup("ECR");
    QString exportFile = m_settings.value("plu_export_file", QDir::homePath()).toString();
    m_settings.endGroup();
    writeExportFile(exportFile);
}

void DialogWarehouse::on_pushButtonSave_clicked()
{
    if (m_currentId > 0) {
        saveCurrentItem();
    } else {
        m_changed = false;
        addNewItem();
    }
    m_completer->refresh();
}

void DialogWarehouse::on_pushButtonAddNew_clicked()
{
    m_currentId = 0;
    ui->nameLineEdit->clear();
    ui->priceInDoubleSpinBox->setValue(0);
    ui->priceOutDoubleSpinBox->setValue(0);
    ui->onStockDoubleSpinBox->setValue(0);
    ui->labelPriceNettoIn->setText("0");
    ui->labelPriceNettoOut->setText("0");
    ui->descriptionTextEdit->clear();
    ui->barcodeLineEdit->clear();
    ui->nameLineEdit->setFocus();
}

void DialogWarehouse::on_pushButtonDeleteCurrent_clicked()
{
    if (QMessageBox::question(this, tr("Are you sure?"), tr("Do you really want to delete this item?")) == QMessageBox::No)
        return;

    if (m_currentId > 0) {
        QSqlQuery query;
        query.prepare("DELETE FROM warehouse_items WHERE id = :id");
        query.bindValue(":id", m_currentId);
        if (query.exec()) {
            reloadModel();
            on_pushButtonAddNew_clicked();
        } else {
            qWarning() << query.lastQuery() << query.lastError().text();
        }
    }
    m_currentId = 0;
}

void DialogWarehouse::on_pushButtonWarehouseBatchItems_clicked()
{
    if (m_currentId > 0)
        batchItemsDialog->showWareHouseItemsBatches(m_currentId, ui->nameLineEdit->text());
}

void DialogWarehouse::reloadModel()
{
    if (m_nameFilter == "") {
        if (m_advancedFilter.isEmpty())
            m_model->setQuery("SELECT id, name, description, price_brutto_in, price_brutto_out, barcode, tax_id, amount FROM warehouse_items ORDER BY " + m_sortField);
        else
            m_model->setQuery("SELECT id, name, description, price_brutto_in, price_brutto_out, barcode, tax_id, amount FROM warehouse_items WHERE "+m_advancedFilter+" ORDER BY " + m_sortField);
    } else {
        if (m_advancedFilter.isEmpty())
            m_model->setQuery("SELECT id, name, description, price_brutto_in, price_brutto_out, barcode, tax_id, amount FROM warehouse_items WHERE name LIKE '%"+m_nameFilter+"%' ORDER BY " + m_sortField);
        else
            m_model->setQuery("SELECT id, name, description, price_brutto_in, price_brutto_out, barcode, tax_id, amount FROM warehouse_items WHERE name LIKE '%"+m_nameFilter+"%' AND "+m_advancedFilter+" ORDER BY " + m_sortField);
    }

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
#else
    ui->tableView->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setResizeMode(4, QHeaderView::Stretch);
#endif
}

void DialogWarehouse::calcNetto()
{
    double priceNettoIn, priceNettoOut;
    priceNettoIn = (ui->priceInDoubleSpinBox->value()-ui->priceInDoubleSpinBox->value()*(ui->taxTypeComboBox->rate()/100.0f));
    priceNettoOut = (ui->priceOutDoubleSpinBox->value()-ui->priceOutDoubleSpinBox->value()*(ui->taxTypeComboBox->rate()/100.0f));
    ui->labelPriceNettoIn->setText(QString::number(priceNettoIn));
    ui->labelPriceNettoOut->setText(QString::number(priceNettoOut));
}

void DialogWarehouse::on_priceInDoubleSpinBox_valueChanged(double arg1)
{
    ui->labelPriceNettoIn->setText(QString::number(arg1 - arg1*(ui->taxTypeComboBox->rate()/100.0f)));
}

void DialogWarehouse::on_priceOutDoubleSpinBox_valueChanged(double arg1)
{
    ui->labelPriceNettoOut->setText(QString::number(arg1 - arg1*(ui->taxTypeComboBox->rate()/100.0f)));
}

void DialogWarehouse::on_taxTypeComboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    calcNetto();
}

void DialogWarehouse::addNewItem()
{
    if (!ui->barcodeLineEdit->text().isEmpty()) {
        if (m_common->sqlHelper()->barcodeExists(ui->barcodeLineEdit->text())) {
            QMessageBox::warning(this,
                                 tr("Conflict"),
                                 tr("Item with barcode %1 already exists").arg(ui->barcodeLineEdit->text()));
            return;
        }
    }

    if (!checkValues()) {
        return;
    }

    WarehouseItemData data;
    data.name = ui->nameLineEdit->text();
    data.description = ui->descriptionTextEdit->toPlainText();
    data.bruttoBuyPrice = ui->priceInDoubleSpinBox->value();
    data.bruttoSellPrice = ui->priceOutDoubleSpinBox->value();
    data.taxId = ui->taxTypeComboBox->currentId();
    data.barcode = ui->barcodeLineEdit->text();
    data.onStock = ui->onStockDoubleSpinBox->value();

    if (m_common->sqlHelper()->addNewItem(data)) {
        setStatusLabel(tr("%1 added").arg(ui->nameLineEdit->text()));
        reloadModel();
        on_pushButtonAddNew_clicked();
    } else {
        setStatusLabel(tr("Failed to add %1").arg(ui->nameLineEdit->text()));
    }
    this->setWindowTitle("");
}

void DialogWarehouse::saveCurrentItem()
{
    // first check that the barcode does not overlaps with some other items
    QSqlQuery query;
    if (!ui->barcodeLineEdit->text().isEmpty()) {
        if (m_common->sqlHelper()->barcodeExists(ui->barcodeLineEdit->text(), m_currentId)) {
            QMessageBox::warning(this,
                                 tr("Conflict"),
                                 tr("Item with barcode %1 already exists").arg(ui->barcodeLineEdit->text()));
            return;
        }
    }

    if (!checkValues()) {
        return;
    }

    if (ui->onStockDoubleSpinBox->value() != m_currentOnStock) {
        double diff = (ui->onStockDoubleSpinBox->value() - m_currentOnStock);
        m_currentOnStock = ui->onStockDoubleSpinBox->value();
        m_common->sqlHelper()->saveAmountChange(m_currentId,  diff, ui->priceOutDoubleSpinBox->value());
    }

    query.prepare("update warehouse_items SET " \
                  "name = :name, " \
                  "description = :description, " \
                  "price_brutto_in = :price_brutto_in, " \
                  "price_brutto_out = :price_brutto_out, " \
                  "tax_id = :tax_id, " \
                  "barcode = :barcode, " \
                  "amount = :amount " \
                  "WHERE id = :id");
    query.bindValue(":name", ui->nameLineEdit->text());
    query.bindValue(":description", ui->descriptionTextEdit->toPlainText());
    query.bindValue(":price_brutto_in", ui->priceInDoubleSpinBox->value());
    query.bindValue(":price_brutto_out", ui->priceOutDoubleSpinBox->value());
    query.bindValue(":tax_id", ui->taxTypeComboBox->currentId());
    query.bindValue(":barcode", ui->barcodeLineEdit->text());
    query.bindValue(":amount", ui->onStockDoubleSpinBox->value());
    query.bindValue(":id", m_currentId);

    if (query.exec()) {
        setStatusLabel(tr("%1 saved successfully").arg(ui->nameLineEdit->text()));
        this->setWindowTitle(ui->nameLineEdit->text());
        m_changed = false;
        reloadModel();
    } else {
        setStatusLabel(tr("%1 save was unsuccessful").arg(ui->nameLineEdit->text()));
        qWarning() << getLastExecutedQuery(query) << query.lastError().text();
    }
}

bool DialogWarehouse::checkValues()
{
    bool ret = true;
    QString errorString = "";
    QWidget *focusWidget = NULL;
    if (ui->priceOutDoubleSpinBox->value() < ui->priceInDoubleSpinBox->value()) {
        ret = false;
        errorString = tr("Item with zero or negative margin?");
        focusWidget = ui->priceOutDoubleSpinBox;
    } else if (ui->nameLineEdit->text().isEmpty()) {
        ret = false;
        errorString = tr("Item must have a name!");
        focusWidget = ui->nameLineEdit;
    } else if (ui->priceOutDoubleSpinBox->value() == 0) {
        ret = false;
        errorString = tr("Item sell price cannot be zero!");
        focusWidget = ui->priceOutDoubleSpinBox;
    }

    if (!ret) {
        QMessageBox::warning(this, tr("Error in input data"), errorString);
        focusWidget->setFocus();
    }

    return ret;
}

void DialogWarehouse::sortItems(int column, Qt::SortOrder order)
{
    m_sortField = m_sortFieldsMap.value(column);
    if (order)
        m_sortField.append(" DESC");
    reloadModel();
}

QString DialogWarehouse::getLastExecutedQuery(const QSqlQuery& query)
{
    QString str = query.lastQuery();
    QMapIterator<QString, QVariant> it(query.boundValues());
    while (it.hasNext())
    {
        it.next();
        str.replace(it.key(),it.value().toString());
    }
    return str;
}

void DialogWarehouse::on_pushButtonHistory_clicked()
{
    if (m_currentId > 0)
        historyDialog->showHistoryOfWarehouseItem(m_currentId);
}


void DialogWarehouse::on_pushButtonExport_clicked()
{
    m_settings.beginGroup("ECR");
    QString exportFile = m_settings.value("plu_export_file", QDir::homePath()).toString();
    m_settings.endGroup();
    if (writeExportFile(exportFile))
        QMessageBox::information(this, tr("PLU file generated"), tr("The PLU file generated to: %1").arg(exportFile));
}


bool DialogWarehouse::writeExportFile(QString fileName)
{
    /** PLU file format:
     * Columns separated by tab ('\t') and newlines terminated with CRLF (\r\n)
     * Columns:
     *  Barcode
     *  Item name
     *  Brutto sell price
     *  Group ID
     *  Amount
     **/

    QFile file(fileName);
    if (file.open(QFile::WriteOnly)) {
        QSqlQuery query;
        query.prepare("SELECT WarehouseItem.barcode, WarehouseItem.name, WarehouseItem.price_brutto_out, Tax.group_id, '0' FROM warehouse_items AS WarehouseItem LEFT JOIN taxes AS Tax ON (WarehouseItem.tax_id = Tax.id) WHERE 1 = 1");
        if (query.exec()) {
            while (query.next()) {
                QString row;
                bool ok = true;
                for (int i = 0; i<query.record().count(); i++) {
                    /**
                     * Do not add items to the PLU list without barcode
                     **/
                    if (i == 0 && query.value(i).toString().isEmpty()) {
                        ok = false;
                    }

                    QString colData = query.value(i).toString();
                    /*if (i == 0) {
                        colData = colData.left(14);
                    } else if (i == 1) {
                        colData = colData.left(16);
                    }*/

                    row = row.append(colData+"\t");
                }

                if (ok)
                    file.write(QString("%1\r\n").arg(row).toLatin1());
            }
        }

        file.close();
        return true;
    } else {
        return false;
    }
}

void DialogWarehouse::setStatusLabel(QString statusText)
{
    QTimer::singleShot(3000, this, SLOT(clearStatusMessage()));
    ui->labelStatusbar->setText(statusText);
}

void DialogWarehouse::on_pushButtonImport_clicked()
{
    m_common->importDetails()->show();
}

void DialogWarehouse::dataChanged()
{
    if (!m_changed) {
        this->setWindowTitle(this->windowTitle()+" *");
        m_changed = true;
    }
}

void DialogWarehouse::on_lineEditFilter_textChanged(const QString &arg1)
{
    m_nameFilter = arg1;
    reloadModel();
}

void DialogWarehouse::on_actionSearch_triggered()
{
    ui->lineEditFilter->selectAll();
    ui->lineEditFilter->setFocus();
}

void DialogWarehouse::tableViewSelectionChanged(const QItemSelection &selected, const QItemSelection &)
{
    if (selected.indexes().size()) {
        QModelIndex index = selected.indexes().first();
        ui->nameLineEdit->setText(m_model->record(index.row()).value("name").toString());
        ui->descriptionTextEdit->setText(m_model->record(index.row()).value("description").toString());
        ui->priceOutDoubleSpinBox->setValue(m_model->record(index.row()).value("price_brutto_out").toDouble());
        ui->priceInDoubleSpinBox->setValue(m_model->record(index.row()).value("price_brutto_in").toDouble());
        ui->barcodeLineEdit->setText(m_model->record(index.row()).value("barcode").toString());
        ui->taxTypeComboBox->setCurrentId(m_model->record(index.row()).value("tax_id").toInt());
        ui->onStockDoubleSpinBox->setValue(m_model->record(index.row()).value("amount").toDouble());
        this->setWindowTitle(ui->nameLineEdit->text());
        calcNetto();

        m_currentId = m_model->record(index.row()).value("id").toInt();
        m_currentOnStock = ui->onStockDoubleSpinBox->value();
        m_changed = false;

        ui->pushButtonSave->setEnabled(true);
    }
}


void DialogWarehouse::clearStatusMessage()
{
    ui->labelStatusbar->clear();
}

void DialogWarehouse::on_tableView_doubleClicked(const QModelIndex &index)
{
    historyDialog->showHistoryOfWarehouseItem(m_model->record(index.row()).value("id").toInt());
}

void DialogWarehouse::on_pushButtonAdvancedFilter_clicked()
{
    advancedFilter->show();
}

void DialogWarehouse::advancedFilterChanged(QString filter)
{
    m_advancedFilter = filter;
    reloadModel();
}
