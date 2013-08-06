#include "dialogecrsettings.h"
#include "ui_dialogecrsettings.h"

#include <QFileDialog>
#include <QDir>

DialogECRSettings::DialogECRSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogECRSettings)
{
    ui->setupUi(this);
    m_settings.beginGroup("ECR");
    ui->pLUExportDirectoryLineEdit->setText(m_settings.value("plu_export_file", QDir::homePath()).toString()); //  FIXME default dir
    ui->pLUReportsDirectoryLineEdit->setText(m_settings.value("plu_reports_file", QDir::homePath()).toString());
    m_settings.endGroup();
}

DialogECRSettings::~DialogECRSettings()
{
    delete ui;
}

void DialogECRSettings::on_pLUReportsDirectoryLineEdit_editingFinished()
{
    m_settings.beginGroup("ECR");
    m_settings.setValue("plu_reports_file", ui->pLUReportsDirectoryLineEdit->text());
    m_settings.endGroup();
}

void DialogECRSettings::on_pLUExportDirectoryLineEdit_editingFinished()
{
    m_settings.beginGroup("ECR");
    m_settings.setValue("plu_export_file", ui->pLUExportDirectoryLineEdit->text());
    m_settings.endGroup();
}

void DialogECRSettings::on_pushButtonBrowseImport_clicked()
{
    QString newPath = QFileDialog::getOpenFileName(this,
                                      tr("Select the PLU report files directory (Data/PLUData.txt folder)"),
                                      ui->pLUReportsDirectoryLineEdit->text(), "(*.txt)");
    if (newPath != "") {
        ui->pLUReportsDirectoryLineEdit->setText(newPath);
        m_settings.beginGroup("ECR");
        m_settings.setValue("plu_reports_file", ui->pLUReportsDirectoryLineEdit->text());
        m_settings.endGroup();
    }
}

void DialogECRSettings::on_pushButtonBrowseExport_clicked()
{
    QString newPath = QFileDialog::getSaveFileName(this,
                                      tr("Select the directory where should we create the *.PLU file"),
                                      ui->pLUExportDirectoryLineEdit->text());
    if (newPath != "") {
        ui->pLUExportDirectoryLineEdit->setText(newPath);
        m_settings.beginGroup("ECR");
        m_settings.setValue("plu_export_file", ui->pLUExportDirectoryLineEdit->text());
        m_settings.endGroup();
    }
}
