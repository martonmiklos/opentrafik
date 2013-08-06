#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QLabel>

#include "dialogaddwarehouse.h"
#include "dialogsellitem.h"
#include "dialogwarehouse.h"
#include "dialogreport.h"
#include "dialogdatabasesettings.h"
#include "dialogguisettings.h"
#include "dialoglogin.h"
#include "dialogabout.h"
#include "dialogusers.h"
#include "ecrdufilereader.h"
#include "systemtray.h"
#include "ecrfilemonitor.h"
#include "dialogecrsettings.h"
#include "dialogtaxedit.h"
#include "dialogcorrector.h"
#include "dialogshopdetails.h"
#include "dialogaddpricebruttoin.h"
#include "dialogimports.h"
#include "dialogmovers.h"
#include "dialogimportdetails.h"
#include "dialogbackup.h"
#include "dialoginvoices.h"
#include "dialogsuppliers.h"
#include "dialogprintlabels.h"

#include "common.h"

#include <QSettings>
#include <QList>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_pushButtonReports_clicked();
    void on_pushButtonWarehouse_clicked();
    void on_actionExit_fullscreen_triggered();
    void on_actionQuit_triggered();
    void on_actionAbout_triggered();
    void on_actionDatabase_settings_triggered();
    void on_actionGUI_settings_triggered();
    void on_dataBaseSettingsDialog_databaseConnected(bool enabled);
    void on_loginDialog_loginSuccessful();
    void on_actionUser_accounts_triggered();
    void on_actionECR_settings_triggered();
    void on_actionTaxes_triggered();
    void on_actionShop_settings_triggered();
    void on_actionAdd_brutto_buy_prices_triggered();
    void on_pushButtonImports_clicked();
    void on_actionMovers_triggered();
    void on_actionBackup_settings_triggered();
    void on_pushButtonInvoices_clicked();
    void on_actionSuppliers_triggered();
    void on_actionPrint_shelf_labels_triggered();

private:
    Ui::MainWindow *ui;

    DialogReport *reportDialog;
    DialogSellItem *sellItemDialog;
    DialogAddWarehouse *warehouseAddDialog;
    DialogWarehouse *warehouseDialog;
    DialogDatabaseSettings *dataBaseSettingsDialog;
    DialogGUISettings *guiSettingsDialog;
    DialogLogin *loginDialog;
    DialogAbout *aboutDialog;
    DialogUsers *usersDialog;
    SystemTray *systemTray;
    ECRFileMonitor *fileMonitor;
    DialogECRSettings *ecrSettingsDialog;
    DialogTaxEdit *taxEditDialog;
    DialogShopDetails *shopDetailsDialog;
    DialogAddPriceBruttoIn *bruttoPriceInAddDialog;
    DialogImports *importsDialog;
    DialogMovers *moversDialog;
    DialogImportFileDetails *importDetails;
    DialogBackup *backupDialog;
    DialogInvoices *invoicesDialog;
    DialogSuppliers *suppliersDialog;
    DialogPrintLabels *labelPrintDialog;

    QSettings m_settings;

    QLabel *statusLabel;

    void loadGUISettings();

    bool m_fullScreen;
    void setFullScreenMode(bool fullscreen);
    bool fullScreenMode() const {return m_fullScreen;}

    //void removeDuplicateImport();

    Common *m_common;
    ECRduFileReader *ecrReader;

    QList<QWidget*> disabledWidgets;
};

#endif // MAINWINDOW_H
