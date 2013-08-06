#ifndef DIALOGGUISETTINGS_H
#define DIALOGGUISETTINGS_H


#include <QDialog>
#include <QTranslator>
#include <QSettings>

namespace Ui {
class DialogGUISettings;
}

class DialogGUISettings : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogGUISettings(QWidget *parent = 0);
    ~DialogGUISettings();
    
private slots:

    void on_buttonBox_accepted();

    void on_comboBoxLanguage_activated(const QString &);

    void on_checkBoxStartMinimized_toggled(bool checked);

private:
    Ui::DialogGUISettings *ui;
    QTranslator m_translator;
    QSettings m_settings;
};

#endif // DIALOGGUISETTINGS_H
