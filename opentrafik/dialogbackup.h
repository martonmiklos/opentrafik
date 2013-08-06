#ifndef DIALOGBACKUP_H
#define DIALOGBACKUP_H

#include <QDialog>
#include <QSettings>
#include <QTimer>

namespace Ui {
class DialogBackup;
}

class DialogBackup : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogBackup(QWidget *parent = 0);
    ~DialogBackup();
    
private slots:
    void on_pushButtonBrowse_clicked();
    void on_checkBox_toggled(bool checked);
    bool backup();

    void on_lineEdit_editingFinished();

    void on_spinBox_valueChanged(int arg1);

    void on_pushButton_clicked();

private:
    Ui::DialogBackup *ui;
    QTimer *backupTimer;
    QSettings settings;
};

#endif // DIALOGBACKUP_H
