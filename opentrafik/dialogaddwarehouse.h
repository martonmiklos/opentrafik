#ifndef DIALOGADDWAREHOUSE_H
#define DIALOGADDWAREHOUSE_H

#include <QDialog>
#include "common.h"

namespace Ui {
class DialogAddWarehouse;
}

class DialogAddWarehouse : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogAddWarehouse(Common *common, QWidget *parent = 0);
    ~DialogAddWarehouse();
    void keyPressEvent(QKeyEvent *event);
private slots:
    void on_barcodeLineEdit_returnPressed();
    void on_amountDoubleSpinBox_editingFinished();
    void on_pushButtonAdd_clicked();
    void clearMessage();
    void removeGreen();
private:
    Ui::DialogAddWarehouse *ui;
    int m_currentAmount;
    QString m_currentItemName;
    Common *m_common;

};

#endif // DIALOGADDWAREHOUSE_H
