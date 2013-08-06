#include "dialogbackup.h"
#include "ui_dialogbackup.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QFileInfo>
#include <QFile>
#include <QDateTime>

DialogBackup::DialogBackup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogBackup)
{
    ui->setupUi(this);
    backupTimer = new QTimer(this);
    connect(backupTimer, SIGNAL(timeout()), this, SLOT(backup()));
    backupTimer->setSingleShot(false);

    settings.beginGroup("backup");
    ui->checkBox->setChecked(settings.value("enabled").toBool());
    ui->lineEdit->setText(settings.value("backupDir").toString());
    ui->spinBox->setValue(settings.value("interval").toInt());

    backupTimer->setInterval(settings.value("interval", 1).toInt()*60000);

    if (settings.value("enabled").toBool())
        backupTimer->start();
    settings.endGroup();
}

DialogBackup::~DialogBackup()
{
    delete ui;
}

void DialogBackup::on_pushButtonBrowse_clicked()
{
    QString backupDir = QFileDialog::getExistingDirectory(this,
                                                          tr("Please select the backup directory!"),
                                                          tr("Select the directory where should I copy the database file periodically"));
    if (!backupDir.isEmpty()) {
        ui->lineEdit->setText(backupDir);
        on_lineEdit_editingFinished();
    }
}

bool DialogBackup::backup()
{
    QString databaseFile, destination;

    settings.beginGroup("database");
    settings.beginGroup("QSQLITE");
    databaseFile = settings.value("database").toString();
    settings.endGroup();
    settings.endGroup();

    QFileInfo fi(databaseFile);
    if (QFile::exists(databaseFile)) {
        destination = ui->lineEdit->text()+"/"+fi.fileName();
        if (QFile::exists(destination)) {
            if (!QFile::remove(destination)) {
                ui->labelLastUpdate->setText(
                            tr("Unable to overwrite the backup file:\n%1\nMaybe it is write protected?").arg(destination));
                return false;
            }
        }

        if (QFile::copy(databaseFile, destination)) {
            ui->labelLastUpdate->setText(
                        tr("Last successful backup:\n%1").arg(QDateTime::currentDateTime().toString("yyyy-M-d h:m:s")));
            return true;
        } else {
            ui->labelLastUpdate->setText(tr("Unable to copy the database file!"));
            return false;
        }
    } else {
        ui->labelLastUpdate->setText(tr("The database file:\n%1\ncould not be found").arg(databaseFile));
    }
    return false;
}

void DialogBackup::on_checkBox_toggled(bool checked)
{
    settings.beginGroup("backup");
    settings.setValue("enabled", checked);
    if (backupTimer->isActive() && !checked)
        backupTimer->stop();
    else if (!backupTimer->isActive() && checked)
        backupTimer->start();
    settings.endGroup();
}

void DialogBackup::on_lineEdit_editingFinished()
{
    settings.beginGroup("backup");
    settings.setValue("backupDir", ui->lineEdit->text());
    settings.endGroup();
}

void DialogBackup::on_spinBox_valueChanged(int arg1)
{
    settings.beginGroup("backup");
    backupTimer->setInterval(arg1*60000);
    settings.setValue("interval", arg1);
    settings.endGroup();
}

void DialogBackup::on_pushButton_clicked()
{
    if (backup()) {
        QMessageBox::information(this,
                                 tr("Backed up successfully"),
                                 tr("Backup was successful!"));
    } else {
        QMessageBox::warning(this,
                             tr("Backed up was not successful"),
                             tr("Backup was unsuccessful!\nPlease check the backup directory if exists!"));
    }
}
