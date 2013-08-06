#include "dialogusers.h"
#include "ui_dialogusers.h"

DialogUsers::DialogUsers(Common *common, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogUsers),
    m_common(common)
{
    ui->setupUi(this);
    addUserDialog = new DialogAddUser(m_common, this);
}

DialogUsers::~DialogUsers()
{
    delete ui;
}

void DialogUsers::on_pushButtonAdd_clicked()
{
    addUserDialog->show();
}
