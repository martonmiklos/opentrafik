#ifndef SQLHELPER_H
#define SQLHELPER_H

#include <QString>
#include <QVariant>
#include <QDateTime>

#include "common.h"

class Common;
struct WarehouseItemData;
struct UserData;
struct SupplierData;

class SqlHelper : public QObject
{
    friend class Common;
public:
    SqlHelper(Common *common);
    WarehouseItemData warehouseItemDataFromBarcode(QString barcode);
    WarehouseItemData warehouseItemDataFromId(quint32 id);

    SupplierData supplierDataFromId(quint32 id);
    bool saveSupplierData(SupplierData data);

    bool saveWareHouseItemData(WarehouseItemData data);
    int barcodeExists(QString barcode, int id = 0, bool *itemIsBatch = NULL, QString *name = NULL);
    bool saveAmountChange(int warehouseItemId, double amountChange, double price, QDateTime date  = QDateTime());
    bool login(QString userName, QString password, bool hashed = false, UserData *userData = NULL);
    bool decreaseItem(QString barcode, double amount, double price, QDateTime date = QDateTime());
    bool addNewItem(WarehouseItemData data);
    QString taxNameFromTaxId(quint32 id);
    bool undoImport(QString date);
    bool substractImports(QString dt1, QString dt2);
    int getItemChangeSum(quint32 id);

private:
    Common *m_common;

};

#endif // SQLHELPER_H
