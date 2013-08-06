#include "dialogmovers.h"
#include "ui_dialogmovers.h"
#include <QDebug>

DialogMovers::DialogMovers(Common *common, QWidget *parent) :
    DialogBaseWithModel(common, parent),
    ui(new Ui::DialogMovers)
{
    ui->setupUi(this);
    m_sort = "ASC";
    ui->tableView->setModel(m_model);

}

DialogMovers::~DialogMovers()
{
    delete ui;
}

void DialogMovers::databaseConnected()
{
    m_model->setQuery("SELECT warehouse_items.name, SUM(amount_change) as sum_change  FROM warehouse_changes  LEFT JOIN warehouse_items ON (warehouse_items.id = warehouse_changes.warehouse_item_id) WHERE amount_change < 0 GROUP BY warehouse_item_id ORDER BY sum_change "+m_sort+" LIMIT 10");
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_model->setHeaderData(0, Qt::Horizontal, tr("Name"));
    m_model->setHeaderData(1, Qt::Horizontal, tr("Move price"));
}

void DialogMovers::on_comboBox_activated(int index)
{
    if (index == 0)
        m_sort = "ASC";
    else
        m_sort = "DESC";
    databaseConnected();
}
