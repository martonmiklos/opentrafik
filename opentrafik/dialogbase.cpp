#include "dialogbase.h"
#include <QDebug>

DialogBase::DialogBase(Common *common, QWidget *parent) :
    QDialog(parent, Qt::Window),
    m_common(common)
{
}

DialogBaseWithModel::DialogBaseWithModel(Common *common, QWidget *parent) :
    DialogBase(common, parent)
{
    m_model = new QSqlQueryModel(this);
}
