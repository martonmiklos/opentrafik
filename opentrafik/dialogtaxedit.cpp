#include "dialogtaxedit.h"
#include "ui_dialogtaxedit.h"

DialogTaxEdit::DialogTaxEdit(Common *common, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTaxEdit),
    m_common(common)
{
    ui->setupUi(this);
}

DialogTaxEdit::~DialogTaxEdit()
{
    delete ui;
}

void DialogTaxEdit::on_tableView_clicked(const QModelIndex &index)
{

}
