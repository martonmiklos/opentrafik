#include "sqlhelper.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QDateTime>
#include <QDebug>
#include <QSqlDriver>

SqlHelper::SqlHelper(Common *common) :
    m_common(common)
{
}

WarehouseItemData SqlHelper::warehouseItemDataFromBarcode(QString barcode)
{
    WarehouseItemData ret;
    ret.id = 0;
    ret.amount = 1.0f;
    ret.barcode = barcode;

    if (barcode.isEmpty())
        return ret;

    QSqlQuery query;
    query.prepare("SELECT id, name, description, amount, price_brutto_out, price_brutto_in, tax_id "
                  "FROM warehouse_items where barcode = :barcode LIMIT 1");
    query.bindValue(":barcode", barcode);
    if (query.exec()) {
        if (!query.next()) {
            query.prepare("SELECT `WarehouseBatchItem`.`warehouse_item_id`, " \
                          "`WarehouseItem`.`name`, " \
                          "`WarehouseItem`.`description`, " \
                          "`WarehouseBatchItem`.`amount`, " \
                          "`WarehouseItem`.`amount`, " \
                          "`WarehouseItem`.`price_brutto_out`, " \
                          "`WarehouseItem`.`price_brutto_in`, " \
                          "`WarehouseItem`.`tax_id` " \
                          "FROM `warehouse_batch_items` AS `WarehouseBatchItem` " \
                          "LEFT JOIN `warehouse_items` AS `WarehouseItem` " \
                          "ON (`WarehouseBatchItem`.`warehouse_item_id` = `WarehouseItem`.`id`) " \
                          "WHERE `WarehouseBatchItem`.`barcode` = :barcode LIMIT 1");
            query.bindValue(":barcode", barcode);
            if(query.exec()) {
                if (query.next()) {
                    ret.id = query.value(0).toInt();
                    ret.name  = query.value(1).toString();
                    ret.description  = query.value(2).toString();
                    ret.amount = query.value(3).toDouble();
                    ret.onStock = query.value(4).toDouble();
                    ret.bruttoSellPrice = query.value(5).toDouble();
                    ret.bruttoBuyPrice = query.value(6).toDouble();
                    ret.taxId = query.value(7).toInt();
                }
            }
        } else {
            ret.id = query.value(0).toInt();
            ret.name  = query.value(1).toString();
            ret.description  = query.value(2).toString();
            ret.onStock = query.value(3).toDouble();
            ret.bruttoSellPrice = query.value(4).toDouble();
            ret.bruttoBuyPrice = query.value(5).toDouble();
            ret.taxId = query.value(6).toInt();
        }
    }
    return ret;
}

WarehouseItemData SqlHelper::warehouseItemDataFromId(quint32 id)
{
    WarehouseItemData ret;
    ret.id = 0;
    ret.amount = 1.0f;

    QSqlQuery query;
    query.prepare("SELECT id, name, description, amount, price_brutto_out, price_brutto_in, tax_id "
                  "FROM warehouse_items where barcode = :barcode LIMIT 1");
    query.bindValue(":id", id);
    if (query.exec()) {
        if (query.next()) {
            ret.id = query.value(0).toInt();
            ret.name  = query.value(1).toString();
            ret.description  = query.value(2).toString();
            ret.onStock = query.value(4).toDouble();
            ret.bruttoSellPrice = query.value(4).toDouble();
            ret.bruttoBuyPrice = query.value(5).toDouble();
            ret.taxId = query.value(6).toInt();
        }
    }
    return ret;
}

SupplierData SqlHelper::supplierDataFromId(quint32 id)
{
    SupplierData ret;
    QSqlQuery query;
    query.prepare("SELECT id, name, address, tax_number, contact_name, contact_email, contact_phone FROM suppliers WHERE id = :id");
    query.bindValue(":id", id);
    if (query.exec() && query.next()) {
        ret.id = query.value("id").toLongLong();
        ret.name = query.value("name").toString();
        ret.address = query.value("address").toString();
        ret.taxNumber = query.value("tax_number").toString();
        ret.contactName = query.value("contact_name").toString();
        ret.contactEmail = query.value("contact_email").toString();
        ret.contactPhone = query.value("contact_phone").toString();
    }
    ret.id = id;
    return ret;
}

bool SqlHelper::saveSupplierData(SupplierData data)
{
    QSqlQuery query;
    if (data.id == 0) {
        query.prepare("INSERT INTO suppliers (name, address, tax_number, contact_name, contact_email, contact_phone) "
                      "VALUES(:name, :address, :tax_number, :contact_name, :contact_email, :contact_phone)");
        query.bindValue(":name", data.name);
        query.bindValue(":address", data.address);
        query.bindValue(":tax_number", data.taxNumber);
        query.bindValue(":contact_name", data.contactName);
        query.bindValue(":contact_email", data.contactEmail);
        query.bindValue(":contact_phone", data.contactPhone);
    } else {
        query.prepare("UPDATE suppliers SET "
                      "name = :name, "
                      "address = :address, "
                      "tax_number = :tax_number, "
                      "contact_name = :contact_name, "
                      "contact_email = :contact_email, "
                      "contact_phone = :contact_phone "
                      "WHERE id = :id");
        query.bindValue(":name", data.name);
        query.bindValue(":address", data.address);
        query.bindValue(":tax_number", data.taxNumber);
        query.bindValue(":contact_name", data.contactName);
        query.bindValue(":contact_email", data.contactEmail);
        query.bindValue(":contact_phone", data.contactPhone);
        query.bindValue(":id", data.id);
    }
    bool ret = query.exec();


    return ret;
}

bool SqlHelper::saveWareHouseItemData(WarehouseItemData data)
{
    if (barcodeExists(data.barcode)) {
        QSqlQuery query;
        query.prepare("update warehouse_items SET " \
                      "name = :name, " \
                      "description = :description, " \
                      "price_brutto_in = :price_brutto_in, " \
                      "price_brutto_out = :price_brutto_out, " \
                      "tax_id = :tax_id, " \
                      "barcode = :barcode, " \
                      "amount = :amount " \
                      "WHERE id = :id");
        query.bindValue(":name", data.name);
        query.bindValue(":description", data.description);
        query.bindValue(":price_brutto_in", data.bruttoBuyPrice);
        query.bindValue(":price_brutto_out", data.bruttoSellPrice);
        query.bindValue(":tax_id", data.taxId);
        query.bindValue(":barcode", data.barcode);
        query.bindValue(":amount", data.amount);
        query.bindValue(":id", data.id);
        if (query.exec()) {
            return true;
        }
    }
    return false;
}

int SqlHelper::barcodeExists(QString barcode, int id, bool *itemIsBatch, QString *name)
{
    if (barcode.isEmpty())
        return false;

    QSqlQuery query;
    if (id == 0)
        query.prepare("SELECT id, name FROM warehouse_items where barcode = :barcode LIMIT 1");
    else  {
        query.prepare("SELECT id, name FROM warehouse_items where barcode = :barcode AND id <> :id LIMIT 1");
        query.bindValue(":id", id);
    }

    query.bindValue(":barcode", barcode);
    query.exec();
    if (query.next()) {
        if (itemIsBatch != NULL)
            *itemIsBatch = false;
        if (name != NULL)
            *name = query.value(1).toString();
        return query.value(0).toInt();
    }

    if (id == 0)
        query.prepare("SELECT b.id, w.name FROM warehouse_batch_items  b, warehouse_items w  "
                      "WHERE b.barcode = :barcode AND w.id = b.warehouse_item_id  LIMIT 1");
    else  {
        query.prepare("SELECT bid,  w.name FROM warehouse_batch_items  b, warehouse_items w "
                      "WHERE b.barcode = :barcode AND id <> :id AND w.id = b.warehouse_item_id LIMIT 1");
        query.bindValue(":id", id);
    }
    query.bindValue(":barcode", barcode);
    query.exec();
    if (query.next()) {
        if (itemIsBatch != NULL)
            *itemIsBatch = true;
        if (name != NULL)
            *name = "";
        if (name != NULL)
            *name = query.value(1).toString();
        return query.value(0).toInt();
    }

    if (name != NULL)
        *name = "";
    return 0;
}

bool SqlHelper::saveAmountChange(int warehouseItemId, double amountChange, double price, QDateTime date)
{
    QSqlQuery query;
    query.prepare("INSERT INTO warehouse_changes (warehouse_item_id, amount_change, date, user_id, price_brutto_out)" \
                  "VALUES(:warehouse_item_id, :amount_change, :date, :user_id, :price_brutto_out)");
    query.bindValue(":warehouse_item_id", warehouseItemId);
    query.bindValue(":amount_change", amountChange);
    query.bindValue(":price_brutto_out", price);

    if (date.isValid())
        query.bindValue(":date", date);
    else
        query.bindValue(":date", QDateTime::currentDateTime());

    query.bindValue(":user_id", m_common->currentUserData().id);
    if (query.exec()) {
        return true;
    }

    qWarning() << query.lastError().text();
    return false;
}


bool SqlHelper::login(QString userName, QString password, bool hashed, UserData *userData)
{
    QString hashedPassword = password;
    if (!hashed)
        hashedPassword = m_common->hashedPassword(password);
    QSqlQuery query;
    query.prepare("SELECT id, name, real_name, role_id FROM users WHERE name = :name AND password = :password");
    query.bindValue(":name", userName);
    query.bindValue(":password", hashedPassword);
    if (query.exec()) {
        if (query.next()) {
            if (userData != NULL) {
                userData->id = query.value(0).toInt();
                userData->name = query.value(1).toString();
                userData->realName = query.value(2).toString();
                userData->roleId = query.value(3).toInt();
            }
            return true;
        }

    }

    return false;
}

bool SqlHelper::decreaseItem(QString barcode, double amount, double price, QDateTime date)
{
    QSqlQuery query;
    bool ok = true;

    if (barcode.isEmpty())
        return false;

    query.prepare("SELECT id, price_brutto_in FROM warehouse_items WHERE barcode = :barcode LIMIT 1");
    query.bindValue(":barcode", barcode);
    if (query.exec()) {
        if (query.next()) {
            if (saveAmountChange(query.value(0).toInt(), amount, price, date)) {
                query.prepare("UPDATE warehouse_items SET amount = amount + :amount WHERE barcode = :barcode");
                query.bindValue(":amount", amount);
                query.bindValue(":barcode", barcode);
                if (!query.exec())
                    ok = false;
            }
        } else {
            ok = false;
        }
    }

    return ok;
}

bool SqlHelper::addNewItem(WarehouseItemData data)
{
    QSqlQuery query;
    query.prepare("INSERT INTO warehouse_items (name, description, price_brutto_in, price_brutto_out, tax_id, barcode, amount) " \
                  "VALUES(:name, :description, :price_brutto_in, :price_brutto_out, :tax_id, :barcode, :amount)");
    query.bindValue(":name", data.name);
    query.bindValue(":description", data.description);
    query.bindValue(":price_brutto_in", data.bruttoBuyPrice);
    query.bindValue(":price_brutto_out", data.bruttoSellPrice);
    query.bindValue(":tax_id", data.taxId);
    query.bindValue(":barcode", data.barcode);
    query.bindValue(":amount", data.onStock);

    if (query.exec()) {
        if (data.onStock != 0) {
            return saveAmountChange(query.lastInsertId().toInt(), data.onStock, data.bruttoSellPrice);
        } else {
            return true;
        }
    } else {
        return false;
    }
}

QString SqlHelper::taxNameFromTaxId(quint32 id)
{
    QSqlQuery query;
    query.prepare("SELECT name FROM taxes WHERE id = :id LIMIT 1");
    query.bindValue(":id", id);
    if (query.exec()) {
        if (query.next()) {
            return query.value(0).toString();
        }
    }
    return "";
}

bool SqlHelper::undoImport(QString date)
{
    QSqlQuery query;
    query.prepare("select amount_change, warehouse_item_id, id from warehouse_changes where datetime(date) = :date AND amount_change < 0");
    query.bindValue(":date", date);
    if (query.exec()) {
        QSqlQuery query2;
        while (query.next()) {
            query2.prepare("UPDATE warehouse_items SET amount = amount - :amount WHERE id = :id");
            qWarning() << query.value(0).toDouble() << query.value(1).toLongLong();
            query2.bindValue(":amount", query.value(0).toDouble());
            query2.bindValue(":id", query.value(1).toLongLong());
            if (!query2.exec())
                return false;

            query2.prepare("delete from warehouse_changes where id = :id");
            query2.bindValue(":id", query.value(2).toLongLong());
            if (!query2.exec())
                return false;
        }
        return true;
    }
    return false;
}

bool SqlHelper::substractImports(QString dt1, QString dt2)
{
    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();

    QSqlQuery query;
    query.prepare("SELECT id, warehouse_item_id, amount_change FROM warehouse_changes WHERE date = :date AND amount_change < 0");
    query.bindValue(":date", dt1);
    if (query.exec()) {
        while (query.next()) {
            QSqlQuery query2;
            query2.prepare("SELECT amount_change, price_brutto_out, price_brutto_in, id FROM warehouse_changes WHERE date = :date2 AND warehouse_item_id = :id");
            query2.bindValue(":id", query.value(1).toLongLong());
            query2.bindValue(":date2", dt2);
            if (query2.exec() && query2.next()) {
                double newAmount = query.value(2).toDouble() - query2.value(0).toDouble();
                QSqlQuery query3;
                if (newAmount == 0) {
                    query3.prepare("DELETE  FROM warehouse_changes WHERE id = :id");
                    query3.bindValue(":id", query.value(0).toLongLong());
                } else {
                    query3.prepare("UPDATE warehouse_changes SET amount_change = :newAmount WHERE id = :id");
                    query3.bindValue(":id",  query.value(0).toLongLong());
                    query3.bindValue(":newAmount",  newAmount);
                }

                if (query3.exec()) {
                    query3.prepare("UPDATE warehouse_items SET amount = (amount + :amount_change) WHERE id = :id");
                    query3.bindValue(":amount_change", -query2.value(0).toDouble());
                    query3.bindValue(":id", query.value(1).toLongLong());
                    if (query3.exec()) {
                    }
                }
            }
        }
    }
    return db.commit();
}

int SqlHelper::getItemChangeSum(quint32 id)
{
    QSqlQuery query;
    query.prepare("SELECT SUM(amount_change) FROM warehouse_changes WHERE warehouse_item_id = :id");
    query.bindValue(":id", id);
    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}
