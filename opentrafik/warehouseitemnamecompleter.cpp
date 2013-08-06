#include "warehouseitemnamecompleter.h"

WarehouseItemNameCompleter::WarehouseItemNameCompleter(QObject *parent) :
    QCompleter(parent)
{
    m_model = new QSqlQueryModel(this);
    this->setModel(m_model);
    refresh();
}

void WarehouseItemNameCompleter::refresh()
{
    m_model->setQuery("SELECT name FROM warehouse_items WHERE 1=1");
}
