#ifndef DIALOGADDINVOICE_H
#define DIALOGADDINVOICE_H

#include <QDialog>

namespace Ui {
class DialogAddInvoice;
}

class DialogAddInvoice : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogAddInvoice(QWidget *parent = 0);
    ~DialogAddInvoice();
    void addInvoiceToSupplier(qlonglong supplierId);
private:
    Ui::DialogAddInvoice *ui;
};

#endif // DIALOGADDINVOICE_H
