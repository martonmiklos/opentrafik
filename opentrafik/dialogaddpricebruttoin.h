#ifndef DIALOGADDPRICEBRUTTOIN_H
#define DIALOGADDPRICEBRUTTOIN_H

#include <QDialog>
#include <QKeyEvent>
#include "common.h"

namespace Ui {
class DialogAddPriceBruttoIn;
}

class DialogAddPriceBruttoIn : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogAddPriceBruttoIn(Common *common, QWidget *parent = 0);
    ~DialogAddPriceBruttoIn();
    void keyPressEvent(QKeyEvent *event);
    
private slots:
    void on_lineEditBarcode_returnPressed();
    void on_pushButtonSave_clicked();

private:
    Ui::DialogAddPriceBruttoIn *ui;
    WarehouseItemData m_currentData;
    Common *m_common;
};

#endif // DIALOGADDPRICEBRUTTOIN_H
