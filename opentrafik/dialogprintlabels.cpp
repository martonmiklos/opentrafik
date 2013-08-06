#include "dialogprintlabels.h"
#include "ui_dialogprintlabels.h"

#include <QPainter>

QVariant PrintItemsModel::data(const QModelIndex &item, int role) const
{
    /*if (item.column() == 0) {
        return true;
        return m_itemsToPrint.at(item.row());
    } else {*/
        return QSqlQueryModel::data(this->index(item.row(), item.column() - 0));
    //}
}

bool PrintItemsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return false;
}

const QModelIndex PrintItemsModel::sqlIndex(const QModelIndex &index)
{
    return this->index(index.row(), index.column() - 1);
}

Qt::ItemFlags PrintItemsModel::flags(const QModelIndex &index) const
{
    /*if (index.column() == 0) {
        return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
    } else {*/
        return QSqlQueryModel::flags(this->index(index.row(), index.column() -0));
    //}
}

int PrintItemsModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return QSqlQueryModel::columnCount(parent) ;
}

DialogPrintLabels::DialogPrintLabels(Common *common, QWidget *parent) :
    DialogBase(common, parent),
    ui(new Ui::DialogPrintLabels)
{
    ui->setupUi(this);
    m_model = new PrintItemsModel(this);
    ui->tableView->setModel(m_model);
}

void DialogPrintLabels::databaseConnected()
{
    m_model->setQuery("SELECT name, price_brutto_out FROm warehouse_items");
   /* m_model->setHeaderData(0, Qt::Horizontal, tr("Name"));
    m_model->setHeaderData(1, Qt::Horizontal, tr("Brutto price"));

    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);*/
}

DialogPrintLabels::~DialogPrintLabels()
{
    delete ui;
}

void DialogPrintLabels::on_pushButtonPreview_clicked()
{
    QPrintDialog *dialog = new QPrintDialog(&printer);
    dialog->setWindowTitle("Print Document");

    if (dialog->exec() != QDialog::Accepted)
        return;
}
