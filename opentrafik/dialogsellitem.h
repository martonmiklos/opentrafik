#ifndef DIALOGSELLITEM_H
#define DIALOGSELLITEM_H

#include <QDialog>

#include "common.h"

namespace Ui {
class DialogSellItem;
}

class DialogSellItem : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogSellItem(Common *common, QWidget *parent = 0);
    ~DialogSellItem();
    
private slots:
    void on_barcodeLineEdit_returnPressed();
    void clearColor();


private:
    Ui::DialogSellItem *ui;
    Common *m_common;
};

#endif // DIALOGSELLITEM_H
