#include "suppliercombobox.h"
#include <QSqlQuery>

SupplierComboBox::SupplierComboBox(QWidget *parent) :
    QComboBox(parent)
{
}

void SupplierComboBox::reload()
{
    this->clear();
    this->addItem(tr("All suppliers"), 0);
    QSqlQuery query;
    query.prepare("SELECT id, name FROM suppliers");
    if (query.exec()) {
        while (query.next()) {
            this->addItem(query.value(1).toString(), query.value(0).toInt());
        }
    }
}
