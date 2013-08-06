#include "dialogguisettings.h"
#include "ui_dialogguisettings.h"
#include <QDebug>
#include <QMessageBox>

DialogGUISettings::DialogGUISettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogGUISettings)
{
    ui->setupUi(this);
    ui->comboBoxLanguage->addItem(tr("Default"), "");
    ui->comboBoxLanguage->addItem(tr("Hungarian"), "hu");

    m_settings.beginGroup("gui");
    ui->comboBoxLanguage->setCurrentIndex(
                ui->comboBoxLanguage->findData(m_settings.value("lang"))
                );
    ui->checkBoxStartMinimized->setChecked(m_settings.value("start_minimized", false).toBool());
    m_settings.endGroup();
}

DialogGUISettings::~DialogGUISettings()
{
    delete ui;
}
void DialogGUISettings::on_buttonBox_accepted()
{
    m_settings.beginGroup("gui");
    m_settings.setValue("lang", ui->comboBoxLanguage->itemData(ui->comboBoxLanguage->currentIndex()));
    m_settings.endGroup();
}

void DialogGUISettings::on_comboBoxLanguage_activated(const QString &)
{
    QMessageBox::information(this,
                             tr("Please restart the application"),
                             tr("Restart to application to load the selected localization!"));
}

void DialogGUISettings::on_checkBoxStartMinimized_toggled(bool checked)
{
    m_settings.beginGroup("gui");
    m_settings.setValue("start_minimized", checked);
    m_settings.endGroup();
}
