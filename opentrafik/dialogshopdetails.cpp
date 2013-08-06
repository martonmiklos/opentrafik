#include "dialogshopdetails.h"
#include "ui_dialogshopdetails.h"

#include <QAbstractButton>

DialogShopDetails::DialogShopDetails(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogShopDetails)
{
    ui->setupUi(this);
    loadSettings();
}

DialogShopDetails::~DialogShopDetails()
{
    delete ui;
}

void DialogShopDetails::on_buttonBox_clicked(QAbstractButton *button)
{
    saveSettings();
}

void DialogShopDetails::loadSettings()
{
    m_settings.beginGroup("shop");
    ui->eCRNumberLineEdit->setText(m_settings.value("ecrNumber").toString());
    ui->textEdit->setPlainText(m_settings.value("shopId").toString());
    ui->taxNumberLineEdit->setText(m_settings.value("taxNumber").toString());
    m_settings.endGroup();
}

void DialogShopDetails::saveSettings()
{
    m_settings.beginGroup("shop");
    m_settings.setValue("ecrNumber", ui->eCRNumberLineEdit->text());
    m_settings.setValue("shopId", ui->textEdit->toPlainText());
    m_settings.setValue("taxNumber", ui->taxNumberLineEdit->text());
    m_settings.endGroup();
}
