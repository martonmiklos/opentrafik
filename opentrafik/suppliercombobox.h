#ifndef SUPPLIERCOMBOBOX_H
#define SUPPLIERCOMBOBOX_H

#include <QComboBox>

class SupplierComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit SupplierComboBox(QWidget *parent = 0);
    void reload();
    qlonglong currentSupplierId() const {return itemData(currentIndex()).toLongLong();}
    
};

#endif // SUPPLIERCOMBOBOX_H
