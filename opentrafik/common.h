#ifndef COMMON_H
#define COMMON_H

#include "sqlhelper.h"
#include "dialogimportdetails.h"
#include "ecrdufilereader.h"

class SqlHelper;
class ECRduFileReader;
class DialogImportFileDetails;

struct WarehouseItemData
{
    QString name, description;
    double amount, bruttoSellPrice, bruttoBuyPrice, onStock;
    quint32 id, taxId;
    QString barcode;
};

struct SupplierData {
    qlonglong id;
    QString name, address, taxNumber, contactName, contactEmail, contactPhone;
};

struct ECRDuFileData
{
    QString barcode, name;
    double amount, price;
};

struct UserData
{
    QString name, realName;
    quint32 id, roleId;
};

class Common : public QObject
{
    Q_OBJECT
public:
    Common(QObject *parent = 0);
    ~Common();

    SqlHelper *sqlHelper() const {return m_sqlHelper;}
    QString salt() const {return "uvahgieTeighogeitoaGhaePhaiLeque";}
    QString hashedPassword(const QString password);
    bool login(QString userName, QString password, bool hashed = false);
    UserData currentUserData() const {return m_currentUserData;}
    ECRduFileReader *ecrDuFileReader() const {return m_ecrDuFileReader;}
    DialogImportFileDetails *importDetails() const {return m_importDetails;}
    void setImportDetails(DialogImportFileDetails *a)  {m_importDetails = a;}
    void askForExit();


private:
    SqlHelper *m_sqlHelper;
    UserData m_currentUserData;
    ECRduFileReader *m_ecrDuFileReader;
    DialogImportFileDetails *m_importDetails;
};

#endif // COMMON_H
