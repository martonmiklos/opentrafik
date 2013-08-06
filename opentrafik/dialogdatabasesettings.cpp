#include "dialogdatabasesettings.h"
#include "ui_dialogdatabasesettings.h"

#include <QCryptographicHash>
#include <QMessageBox>
#include <QFile>
#include <QDebug>

DialogDatabaseSettings::DialogDatabaseSettings(Common *common, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDatabaseSettings),
    m_common(common)
{
    this->setObjectName("dataBaseSettingsDialog");
    ui->setupUi(this);
    ui->databaseTypeComboBox->addItems(QSqlDatabase::drivers());
    m_settings.beginGroup("database");
    QString databaseType = m_settings.value("database_driver", "QSQLITE").toString();
    ui->databaseTypeComboBox->setCurrentIndex(ui->databaseTypeComboBox->findText(databaseType));
    m_settings.endGroup();

    fillInputs(ui->databaseTypeComboBox->currentText());
}

DialogDatabaseSettings::~DialogDatabaseSettings()
{
    delete ui;
}

void DialogDatabaseSettings::on_buttonBox_accepted()
{
    m_settings.beginGroup("database");
    m_settings.setValue("database_driver", ui->databaseTypeComboBox->currentText());
    m_settings.beginGroup(ui->databaseTypeComboBox->currentText());
    m_settings.setValue("server", ui->serverLineEdit->text());
    m_settings.setValue("database", ui->databaseNameLineEdit->text());
    m_settings.setValue("username", ui->usernameLineEdit->text());
    m_settings.setValue("password", ui->passwordLineEdit->text().toLatin1().toBase64());
    m_settings.endGroup();
    m_settings.endGroup();
    m_settings.sync();

    if (m_dataBaseInitialized) {
        QMessageBox::information(this,
                                 tr("Please restart the application"),
                                 tr("To use the new database settings please restart the application!"));
    } else {
        initDatabase();
    }
}

void DialogDatabaseSettings::on_databaseTypeComboBox_activated(const QString &arg1)
{
    fillInputs(arg1);
}

void DialogDatabaseSettings::fillInputs(QString driverType)
{
    bool isSQLite = driverType == "QSQLITE";
    ui->serverLineEdit->setEnabled(!isSQLite);
    m_settings.beginGroup("database");
    m_settings.beginGroup(driverType);
    ui->serverLineEdit->setText(m_settings.value("server", "").toString());
    ui->databaseNameLineEdit->setText(m_settings.value("database").toString());
    ui->usernameLineEdit->setText(m_settings.value("username", "").toString());
    ui->passwordLineEdit->setText(QByteArray::fromBase64(m_settings.value("password", "").toByteArray()));
    m_settings.endGroup();
    m_settings.endGroup();
}


bool DialogDatabaseSettings::initDatabase()
{
    bool ret = true;
    db = QSqlDatabase::database();
    m_settings.beginGroup("database");
    QString databaseType = m_settings.value("database_driver", "QSQLITE").toString();
    m_settings.beginGroup(databaseType);
    if (databaseType == "QSQLITE") {
        if (!QFile::exists(m_settings.value("database", "database.sqlite").toString())) {
            QMessageBox::warning(this,
                                 tr("Error"),
                                 tr("The %1 database file not found!").arg(m_settings.value("database").toString()));
            ret = false;
        }
    }

    if (ret) {
        db = QSqlDatabase::addDatabase(databaseType);
        if (!db.isValid()) {
            QMessageBox::warning(this,
                                 tr("Database driver error"),
                                 tr("Unable to initialize the %1 database driver\n" \
                                    "Please check your database configuration!").arg(databaseType),
                                 QMessageBox::Ok);
            ret = false;
        } else {
            db.setHostName(m_settings.value("server").toString()); // FIXME
            db.setDatabaseName(m_settings.value("database").toString());
            db.setUserName(m_settings.value("username", "opentrafik").toString());
            db.setPassword(QByteArray::fromBase64(m_settings.value("password").toByteArray()));
            if (!db.open()) {
                QMessageBox::warning(this,
                                     tr("Unable to connect to the database"),
                                     tr("The OpenTrafik was unable to connect to the database\n" \
                                        "Please check your database configuration!"),
                                     QMessageBox::Ok);
                ret = false;
            }
        }
    }
    m_settings.endGroup();
    m_settings.endGroup();
    emit databaseConnected(ret);
    m_dataBaseInitialized = ret;
    if (!ret)
        this->show();
    return ret;
}

void DialogDatabaseSettings::on_pushButtonTest_clicked()
{

}
