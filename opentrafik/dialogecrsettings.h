#ifndef DIALOGECRSETTINGS_H
#define DIALOGECRSETTINGS_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class DialogECRSettings;
}

class DialogECRSettings : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogECRSettings(QWidget *parent = 0);
    ~DialogECRSettings();
    
private slots:
    void on_pLUReportsDirectoryLineEdit_editingFinished();

    void on_pLUExportDirectoryLineEdit_editingFinished();

    void on_pushButtonBrowseImport_clicked();

    void on_pushButtonBrowseExport_clicked();

private:
    Ui::DialogECRSettings *ui;
    QSettings m_settings;
};

#endif // DIALOGECRSETTINGS_H
