#include "taxcombobox.h"
#include <QDebug>
#include <QSqlQuery>

TaxComboBox::TaxComboBox(QWidget *parent) :
    QComboBox(parent)
{
}

TaxComboBox::~TaxComboBox()
{
}

void TaxComboBox::reload()
{
    QSqlQuery m_query;
    m_query.prepare("SELECT id, name, rate FROM taxes");
    m_query.exec();
    clear();
    m_rates.clear();
    while (m_query.next()) {
        this->addItem(m_query.value(1).toString(), m_query.value(0));
        m_rates.append(m_query.value(2).toInt());
    }
}

double TaxComboBox::rate(int id)
{
    if (id < 0) {
        return m_rates.value(this->currentIndex());
    }

    int index = this->findData(id);
    if (index < 0)
        return 0;
    return m_rates.at(index);
}

void TaxComboBox::setCurrentId(int id)
{
    setCurrentIndex(findData(id));
}

int TaxComboBox::currentId()
{
    return this->itemData(this->currentIndex()).toInt();
}
