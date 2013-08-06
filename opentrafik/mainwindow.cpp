#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QLabel>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlResult>
#include <QFile>
#include <QFileInfo>
#include <QTranslator>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    usersDialog(NULL)
{
    m_common = new Common(this);

    systemTray = new SystemTray(m_common, this);
    connect(systemTray, SIGNAL(mainWindowRequested()), this, SLOT(show()));

    dataBaseSettingsDialog = new DialogDatabaseSettings(m_common, this);
    warehouseDialog = new DialogWarehouse(m_common, this);
    warehouseAddDialog = new DialogAddWarehouse(m_common, this);
    sellItemDialog = new DialogSellItem(m_common, this);
    reportDialog = new DialogReport(m_common, this);
    guiSettingsDialog = new DialogGUISettings(this);
    loginDialog = new DialogLogin(m_common, this);
    aboutDialog = new DialogAbout(this);
    ecrSettingsDialog = new DialogECRSettings(this);
    taxEditDialog = new DialogTaxEdit(m_common, this);
    shopDetailsDialog = new DialogShopDetails(this);
    bruttoPriceInAddDialog = new DialogAddPriceBruttoIn(m_common, this);
    importsDialog = new DialogImports(m_common, this);
    moversDialog = new DialogMovers(m_common, this);
    importDetails = new DialogImportFileDetails(m_common, this);
    backupDialog = new DialogBackup(this);
    invoicesDialog = new DialogInvoices(m_common, this);
    suppliersDialog = new DialogSuppliers(m_common, this);
    labelPrintDialog = new DialogPrintLabels(m_common, this);
    //labelPrintDialog->show();

    ui->setupUi(this);
    disabledWidgets << /*ui->pushButtonFillWarehouse << */ui->pushButtonReports << /*ui->pushButtonSellItems <<*/ ui->pushButtonImports << ui->pushButtonWarehouse;
    statusLabel = new QLabel(this);
    ui->statusBar->addWidget(statusLabel);

    dataBaseSettingsDialog->initDatabase();

    connect(m_common->ecrDuFileReader(), SIGNAL(fileReaded(QString, double)), systemTray,SLOT(fileImported(QString, double)));
    connect(m_common->ecrDuFileReader(), SIGNAL(fileReadFailed(QString)), systemTray,SLOT(fileImportFailed(QString)));
    connect(m_common->ecrDuFileReader(), SIGNAL(fileReaded(QString, double)), warehouseDialog, SLOT(exportPLUFile()));
    connect(importDetails, SIGNAL(fileReaded()), warehouseDialog, SLOT(exportPLUFile()));

    fileMonitor = new ECRFileMonitor(this);
    connect(fileMonitor, SIGNAL(newFileImported(QString)), m_common->importDetails(), SLOT(importFile(QString)));
}

void MainWindow::loadGUISettings()
{
    m_settings.beginGroup("gui");
    m_fullScreen = m_settings.value("fullScreen", false).toBool();
    m_settings.endGroup();
}

MainWindow::~MainWindow()
{
    m_settings.sync();
    delete ui;
}

void MainWindow::on_pushButtonReports_clicked()
{
    if (m_fullScreen)
        reportDialog->showMaximized();
    else
        reportDialog->show();
}

void MainWindow::on_pushButtonWarehouse_clicked()
{
    if (m_fullScreen)
        warehouseDialog->showMaximized();
    else
        warehouseDialog->show();
}

void MainWindow::on_actionExit_fullscreen_triggered()
{
    m_fullScreen = !m_fullScreen;
    setFullScreenMode(m_fullScreen);
}

void MainWindow::on_actionQuit_triggered()
{
    m_common->askForExit();
}
/*
void MainWindow::removeDuplicateImport()
{
    QSqlQuery query;
    query.prepare("SELECT id, name, amount FROM warehouse_items WHERE 1=1");
    if (query.exec()) {
        while (query.next()) {
            QSqlQuery query2;
            query2.prepare("SELECT SUM(amount_change) FROM warehouse_changes WHERE warehouse_item_id=:id");
            query2.bindValue(":id", query.value(0).toInt());

            if (query2.exec()) {
                if (query2.next()) {
                    if ((query2.value(0).toDouble() != query.value(2).toDouble()) && (query2.value(0).toInt() < 0)) {
                        double newAmount = query2.value(0).toDouble();
                        qWarning() << query.value(1).toString()
                                   << "original: "
                                   << query.value(2).toDouble()
                                      << "-> "
                                   << query2.value(0).toDouble();
                        QSqlQuery query3;
                        query3.prepare("UPDATE warehouse_items SET amount=:amount WHERE id=:id");
                        query3.bindValue(":amount", newAmount);
                        query3.bindValue(":id", query.value(0).toInt());
                        if (!query3.exec()) {
                            qWarning() << query3.lastQuery();
                        }
                    }
                }
            }
        }
    }
}*/

void MainWindow::on_dataBaseSettingsDialog_databaseConnected(bool ok)
{
    foreach (QWidget *widget, disabledWidgets) {
        widget->setEnabled(ok);
    }

    if (ok) {
        //substractImports();
        statusLabel->setText(tr("Database: %1 connected ok").arg(QSqlDatabase::database().driverName()));
        warehouseDialog->databaseConnected();
        reportDialog->databaseConnected();
        importsDialog->databaseConnected();
        moversDialog->databaseConnected();
        invoicesDialog->databaseConnected();
        labelPrintDialog->databaseConnected();
        if (!loginDialog->autoLogin()) {
            loginDialog->show();
        } else {
            on_loginDialog_loginSuccessful();
        }
    } else {
        statusLabel->setText(tr("Database: %1 connection failed").arg(QSqlDatabase::database().driverName()));
        dataBaseSettingsDialog->show();
    }
}

void MainWindow::on_loginDialog_loginSuccessful()
{
    loadGUISettings();
    m_settings.beginGroup("gui");
    if (!m_settings.value("start_minimized", false).toBool())
        setFullScreenMode(m_fullScreen);
    m_settings.endGroup();
    loginDialog->close();

    if (m_common->currentUserData().roleId == 1) {
        ui->actionUser_accounts->setVisible(true);
        ui->actionUser_accounts->setEnabled(true);
        usersDialog = new DialogUsers(m_common, this);
    }
}

void MainWindow::setFullScreenMode(bool fullscreen)
{
    if (m_fullScreen != fullscreen) {
        m_fullScreen = fullscreen;
        m_settings.beginGroup("gui");
        m_settings.setValue("fullScreen", m_fullScreen);
        m_settings.endGroup();
    }

    if (m_fullScreen) {
        this->showFullScreen();
    } else {
        this->showNormal();
    }

    if (fullscreen)
        ui->actionExit_fullscreen->setText(tr("Exit fullscreen mode"));
    else
        ui->actionExit_fullscreen->setText(tr("Fullscreen mode"));
}

void MainWindow::on_actionAbout_triggered()
{
   aboutDialog->show();
}

void MainWindow::on_actionDatabase_settings_triggered()
{
    dataBaseSettingsDialog->show();
}

void MainWindow::on_actionGUI_settings_triggered()
{
    guiSettingsDialog->show();
}

void MainWindow::on_actionUser_accounts_triggered()
{
    if (usersDialog != NULL) {
        usersDialog->show();
    }
}

void MainWindow::on_actionECR_settings_triggered()
{
    ecrSettingsDialog->show();
}

void MainWindow::on_actionTaxes_triggered()
{
    taxEditDialog->show();
}

/*void MainWindow::on_actionCorrect_barcodes_triggered()
{
    correctorDialog->show();
}*/

void MainWindow::on_actionShop_settings_triggered()
{
    shopDetailsDialog->show();
}

void MainWindow::on_actionAdd_brutto_buy_prices_triggered()
{
    bruttoPriceInAddDialog->show();
}

void MainWindow::on_pushButtonImports_clicked()
{
    importsDialog->show();
}

void MainWindow::on_actionMovers_triggered()
{
    moversDialog->show();
}


void MainWindow::on_actionBackup_settings_triggered()
{
    backupDialog->show();
}

void MainWindow::on_pushButtonInvoices_clicked()
{
    invoicesDialog->show();
}

void MainWindow::on_actionSuppliers_triggered()
{
    suppliersDialog->show();
}

void MainWindow::on_actionPrint_shelf_labels_triggered()
{
    labelPrintDialog->show();
}
