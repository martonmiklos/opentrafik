#include "dialogimportdetails.h"
#include "ui_dialogimportdetails.h"

#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include <QSqlDatabase>
#include <QMessageBox>

DialogImportFileDetails::DialogImportFileDetails(Common *common, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogImportDetails),
    m_common(common)
{
    ui->setupUi(this);
    m_common->setImportDetails(this);
    ui->dateEdit->setDate(QDate::currentDate());
    m_model = new ECRDUFileModel(m_common, this);
    ui->tableView->setModel(m_model);
}

DialogImportFileDetails::~DialogImportFileDetails()
{
    delete ui;
}

void DialogImportFileDetails::importFile(QString fileName)
{
    ui->lineEditFile->setText(fileName);
    m_model->readFile(fileName);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);

    ui->labelAmountSum->setText(QString("%1 pcs.").arg(m_model->amountChangeSum()));
    ui->labelPriceSum->setText(QString("%1 Ft").arg(m_model->priceChangeSum()));
    this->show();
}

void DialogImportFileDetails::on_pushButtonBrowse_clicked()
{
    m_settings.beginGroup("ECR");
    QString dirString(m_settings.value("plu_reports_file", QDir::homePath()).toString());
    m_settings.endGroup();

    QFileInfo fi(dirString);
    QDir dir(fi.path());
    dir.cdUp();
    dir.cd("duFile");
    QString importFileName = QFileDialog::getOpenFileName(this, tr("Select PLU file"), dir.path(), "(*.hex)");
    if (importFileName != "") {
        importFile(importFileName);
    }
}

void DialogImportFileDetails::on_pushButtonImport_clicked()
{
    if (!ui->lineEditFile->text().isEmpty()) {
        if (m_model->saveToDatabase(ui->dateEdit->date())) {
            QMessageBox::information(this,
                                     tr("Success"),
                                     tr("%1 items imported successfully, %2 not")
                                        .arg(m_model->amountChangeSum()).arg(m_model->failedCnt()));
        } else {
            QMessageBox::warning(this, tr("Shit happened"), tr("Import was unsuccessful please contact with the support!"));
        }

        emit fileReaded();
    }
}

ECRDUFileModel::ECRDUFileModel(Common *common, QObject *parent) :
    QAbstractTableModel(parent),
    m_common(common)
{
}

bool ECRDUFileModel::readFile(QString fileName)
{
    bool ret = false;
    beginResetModel();
    ret = m_common->ecrDuFileReader()->readFile(fileName, &m_data);
    endResetModel();

    m_priceChangeSum = 0;
    m_amountChangeSum = 0;
    foreach (ECRDuFileData data, m_data) {
        m_priceChangeSum += data.amount * data.price;
        m_amountChangeSum += data.amount;
    }
    return ret;
}

QVariant ECRDUFileModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0:
            return tr("Name");
            break;
        case 1:
            return tr("Price");
            break;
        case 2:
            return tr("Amount");
            break;
        }
    }
    return QVariant();
}

QVariant ECRDUFileModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
        case 0:
            return m_data.at(index.row()).name;
            break;
        case 1:
            return m_data.at(index.row()).price;
            break;
        case 2:
            return m_data.at(index.row()).amount;
            break;
        }
    }
    return QVariant();
}

bool ECRDUFileModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
         return false;

    if (index.column() == 2) { // only the amount is editable
        m_data[index.row()].amount = value.toInt();
    }
    return false;
}

Qt::ItemFlags ECRDUFileModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags ret = Qt::ItemIsEnabled;
    if (index.column() == 2)
        ret |= Qt::ItemIsEditable;

    return ret;
}

int ECRDUFileModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_data.size();
}

int ECRDUFileModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return 3;
}

bool ECRDUFileModel::saveToDatabase(QDate date)
{
    m_failedCnt = 0;

    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();
    foreach (ECRDuFileData data, m_data) {
        if (!m_common->sqlHelper()->decreaseItem(data.barcode, data.amount, data.price, QDateTime(date))) {
            qWarning() << data.name << data.barcode << data.amount;
            m_failedCnt++;
        }
    }

    return db.commit();
}
