#ifndef DIALOGSHOPDETAILS_H
#define DIALOGSHOPDETAILS_H

#include <QDialog>
#include <QSettings>
#include <QAbstractButton>

namespace Ui {
class DialogShopDetails;
}

class DialogShopDetails : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogShopDetails(QWidget *parent = 0);
    ~DialogShopDetails();
    
private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::DialogShopDetails *ui;
    void loadSettings();
    void saveSettings();
    QSettings m_settings;
};

#endif // DIALOGSHOPDETAILS_H
