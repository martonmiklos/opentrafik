#ifndef DIALOGDATABASESETTINGS_H
#define DIALOGDATABASESETTINGS_H

#include <QDialog>
#include <QSettings>
#include <QSqlDatabase>

#include "common.h"

namespace Ui {
class DialogDatabaseSettings;
}

class DialogDatabaseSettings : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogDatabaseSettings(Common *common, QWidget *parent = 0);
    ~DialogDatabaseSettings();
    bool initDatabase();

signals:
    void databaseConnected(bool connected);

private slots:
    void on_buttonBox_accepted();
    void on_databaseTypeComboBox_activated(const QString &arg1);
    void fillInputs(QString driverType);

    void on_pushButtonTest_clicked();

private:
    Ui::DialogDatabaseSettings *ui;
    QSettings m_settings;
    Common *m_common;
    QSqlDatabase db;
    bool m_dataBaseInitialized;
};

#endif // DIALOGDATABASESETTINGS_H
