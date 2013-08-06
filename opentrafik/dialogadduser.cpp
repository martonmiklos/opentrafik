#include "dialogadduser.h"
#include "ui_dialogadduser.h"
#include <QMessageBox>

DialogAddUser::DialogAddUser(Common *common, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddUser),
    m_common(common)
{
    ui->setupUi(this);
}

DialogAddUser::~DialogAddUser()
{
    delete ui;
}

void DialogAddUser::on_pushButtonAdd_clicked()
{
    if (ui->loginNameLineEdit->text().isEmpty()) {
        ui->loginNameLineEdit->setFocus();
        inputDataError(tr("The login name cannot left blank!"));
        return;
    }

    if (ui->realNameLineEdit->text().isEmpty()) {
        ui->realNameLineEdit->setFocus();
        inputDataError(tr("The real name cannot left blank!"));
        return;
    }

    if (ui->passwordAgainLineEdit->text().isEmpty()) {
        ui->passwordAgainLineEdit->setFocus();
        inputDataError(tr("The password cannot left blank!"));
        return;
    }

    if (ui->passwordLineEdit->text() != ui->passwordAgainLineEdit->text()) {
        ui->passwordLineEdit->clear();
        ui->passwordAgainLineEdit->clear();
        ui->passwordLineEdit->setFocus();
        inputDataError(tr("The passwords does not match!"));
        return;
    }
}

void DialogAddUser::inputDataError(QString error)
{
    QMessageBox::warning(this,
                         tr("Error in the input data"),
                         error);
}
