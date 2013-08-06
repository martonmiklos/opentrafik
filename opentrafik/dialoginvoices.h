#ifndef DIALOGINVOICES_H
#define DIALOGINVOICES_H

#include "dialogbase.h"
#include "dialogaddinvoice.h"


namespace Ui {
class DialogInvoices;
}

class DialogInvoices : public DialogBaseWithModel
{
    Q_OBJECT
    
public:
    explicit DialogInvoices(Common *common, QWidget *parent = 0);
    ~DialogInvoices();
    void showEvent(QShowEvent *);
    void showSuppliersInvoices(quint32 supplierId);
    void databaseConnected();
    void reloadModel();
    
private:
    QString m_completeFilter, m_supplierFilter;

private slots:
    void on_comboBoxSupplier_currentIndexChanged(int index);
    void on_checkBoxIncomplete_toggled(bool checked);

    void on_pushButtonNew_clicked();

private:
    Ui::DialogInvoices *ui;
    DialogAddInvoice *addInvoiceDialog;
};

#endif // DIALOGINVOICES_H
