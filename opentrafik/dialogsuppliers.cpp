#include "dialogsuppliers.h"
#include "ui_dialogsuppliers.h"

#include <QSqlRecord>
#include <QMessageBox>
#include <QDebug>

DialogSuppliers::DialogSuppliers(Common *common, QWidget *parent) :
    DialogBaseWithModel(common, parent),
    ui(new Ui::DialogSuppliers),
    m_modified(false)
{
    ui->setupUi(this);
    m_currentEditId = 0;
    ui->tableView->setModel(m_model);
    ui->tableView->setSelectionMode(QTableView::SingleSelection);
    ui->tableView->setSelectionBehavior(QTableView::SelectRows);

    connect(ui->nameLineEdit, SIGNAL(textEdited(QString)), this, SLOT(dataChanged()));
    connect(ui->addressTextEdit, SIGNAL(textChanged()), this, SLOT(dataChanged()));
    connect(ui->taxNumberLineEdit, SIGNAL(textEdited(QString)), this, SLOT(dataChanged()));
    connect(ui->contactNameLineEdit, SIGNAL(textEdited(QString)), this, SLOT(dataChanged()));
    connect(ui->contactEMailAddressLineEdit, SIGNAL(textEdited(QString)), this, SLOT(dataChanged()));
    connect(ui->contactTelephoneLineEdit, SIGNAL(textEdited(QString)), this, SLOT(dataChanged()));
}

DialogSuppliers::~DialogSuppliers()
{
    delete ui;
}

void DialogSuppliers::databaseConnected()
{

}

void DialogSuppliers::showEvent(QShowEvent *)
{
    reloadModel();
}

void DialogSuppliers::reloadModel()
{
    m_model->setQuery("SELECT id, name, tax_number, contact_name FROM suppliers");
}

void DialogSuppliers::on_pushButtonSave_clicked()
{
    SupplierData data;
    data.id = m_currentEditId;
    data.name = ui->nameLineEdit->text();
    data.taxNumber = ui->taxNumberLineEdit->text();
    data.contactName = ui->contactNameLineEdit->text();
    data.contactEmail = ui->contactEMailAddressLineEdit->text();
    data.contactPhone = ui->contactTelephoneLineEdit->text();
    if (m_common->sqlHelper()->saveSupplierData(data)) {
        QString operation = tr("saved");
        if (data.id == 0) operation = tr("added");
        ui->labelStatus->setText(tr("The %1 supplier %2 successfully").arg(data.name).arg(operation));
        ui->pushButtonSave->setEnabled(false);
        this->setWindowTitle(tr("Suppliers"));
        m_modified = false;
    } else {
        ui->labelStatus->setText(tr("The save was unsuccessful"));
    }
}

void DialogSuppliers::on_pushButtonNew_clicked()
{
    if (loadNewAllowed()) {
        disconnect(ui->addressTextEdit, SIGNAL(textChanged()), this, SLOT(dataChanged()));
        ui->nameLineEdit->clear();
        ui->taxNumberLineEdit->clear();
        ui->contactEMailAddressLineEdit->clear();
        ui->contactTelephoneLineEdit->clear();
        ui->addressTextEdit->clear();
        ui->contactNameLineEdit->clear();
        m_currentEditId = 0;
        ui->pushButtonSave->setEnabled(true);
        connect(ui->addressTextEdit, SIGNAL(textChanged()), this, SLOT(dataChanged()));
    }
}


void DialogSuppliers::on_tableView_activated(const QModelIndex &index)
{
    if (loadNewAllowed()) {
        disconnect(ui->addressTextEdit, SIGNAL(textChanged()), this, SLOT(dataChanged()));
        SupplierData data = m_common->sqlHelper()->supplierDataFromId(m_model->record(index.row()).value(0).toLongLong());
        ui->nameLineEdit->setText(data.name);
        ui->taxNumberLineEdit->setText(data.taxNumber);
        ui->contactEMailAddressLineEdit->setText(data.contactEmail);
        ui->contactTelephoneLineEdit->setText(data.contactPhone);
        ui->addressTextEdit->setText(data.address);
        ui->contactNameLineEdit->setText(data.contactName);
        m_modified = false;
        m_currentEditId = data.id;
        ui->pushButtonSave->setEnabled(false);
        this->setWindowTitle(tr("Suppliers"));
        connect(ui->addressTextEdit, SIGNAL(textChanged()), this, SLOT(dataChanged()));
    }
}

void DialogSuppliers::dataChanged()
{
    if (!m_modified) {
        this->setWindowTitle(this->windowTitle()+" *");
        m_modified = true;
        ui->pushButtonSave->setEnabled(true);
    }
}

bool DialogSuppliers::loadNewAllowed()
{
    bool load = true;
    if (m_modified) {
        QMessageBox::StandardButton btn = QMessageBox::question(
                    this,
                    tr("Discard changes?"),
                    tr("The details of the supplier: \n%1\n had been changed\n do you want to save these changes?")
                    .arg(ui->nameLineEdit->text()),
                    QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                    QMessageBox::Discard);
        if (btn == QMessageBox::Cancel) {
            load = false;
        } else if (btn == QMessageBox::Save) {
            on_pushButtonSave_clicked();
        }
    }
    return load;
}
