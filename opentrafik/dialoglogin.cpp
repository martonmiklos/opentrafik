#include "dialoglogin.h"
#include "ui_dialoglogin.h"
#include <QMessageBox>
#include <QShowEvent>

DialogLogin::DialogLogin(Common *common, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogLogin),
    m_common(common)
{
    this->setObjectName("loginDialog");
    ui->setupUi(this);
}

DialogLogin::~DialogLogin()
{
    delete ui;
}

bool DialogLogin::autoLogin()
{
    bool ret = false;
    m_settings.beginGroup("login");
    if (m_settings.value("autologin", false).toBool()) {
        if (m_common->login(m_settings.value("username").toString(),
                                         m_settings.value("password").toString(),
                                         true)) {
            ret = true;
        } else {
            showLoginError();
        }
    }
    m_settings.endGroup();
    return ret;
}

void DialogLogin::on_pushButton_clicked()
{
    if (m_common->login(ui->usernameLineEdit->text(), ui->passwordLineEdit->text())) {
        if (ui->rememberMeCheckBox->isChecked()) {
            m_settings.beginGroup("login");
            m_settings.setValue("autologin", true);
            m_settings.setValue("username", ui->usernameLineEdit->text());
            m_settings.setValue("password", m_common->hashedPassword(ui->passwordLineEdit->text()));
            m_settings.endGroup();
        }
        this->close();
        emit loginSuccessful();
    } else {
        showLoginError();
        ui->passwordLineEdit->setText("");
        ui->passwordLineEdit->setFocus();
    }
}

void DialogLogin::showLoginError()
{
    QMessageBox::warning(this,
                         tr("Login error"),
                         tr("Invalid username and/or password"));
}
