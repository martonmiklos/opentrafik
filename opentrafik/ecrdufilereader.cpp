#include "ecrdufilereader.h"
#include <QFile>
#include <QDebug>
#include <QSqlDatabase>

/**
 * @brief ECRduFileReader is a class for read the daily PLU report files of
 * produced by the WisDor ECR's utility.
 * @param parent
 */

ECRduFileReader::ECRduFileReader(Common *common) :
    QObject(common),
    m_common(common)
{
}

/**
 * @brief ECRduFileReader::readFile
 * @param filename
 * @return returns false if the file cannot be opened for reading or does not have the first 0x100 byte long header
 */
bool ECRduFileReader::readFile(QString filename, QDate date)
{
    QFile file(filename);
    double priceSum = 0;
    if (file.open(QFile::ReadOnly)) {
        /**
         * The data file first 0x100 byte is a header,
         * the PLUs data follows it in the following format:
         * 0x00:0x06    (7 bytes):  barcode in gray format
         * 0x07:0x16    (16 bytes): item name
         * 0x17         (1 byte):   0x00 (terminating \0 character to the string?)
         * 0x18:0x1B    (4 byte):   price in int32 format (big endian)
         * 0x1C:0x20    (4 byte):   amount*1000 in int32 format (big endian)
         */
        if (!file.seek(0x100)) {
            emit fileReadFailed(filename);
            return false;
        }

        QSqlDatabase db = QSqlDatabase::database();
        db.transaction();

        while (!file.atEnd()) {
            QByteArray temp = file.read(32);
            QString barCode, name;
            float amount = 0;
            quint32 price = 0;

            quint32 sum = 0;
            for (int i = 0x00; i<0x07; i++) {
                sum += (quint8)temp.at(i);
                if (sum) {
                    if (barCode.isEmpty())
                        barCode = barCode.append(QString::number((quint8)temp.at(i), 16));
                    else
                        barCode = barCode.append(QString::number((quint8)temp.at(i), 16).rightJustified(2, '0'));
                }
            }

            for (int i = 0x07; i<0x17; i++) {
                if (temp.at(i) != 0)
                    name = name.append(QChar(temp.at(i)));
            }

            int intTemp = 0, j = 2;
            for (int i = 0x18; i<0x1B; i++) {
                price += ((quint8)temp.at(i) << j*8);
                j--;
            }

            j = 3;
            for (int i = 0x1C; i<0x20; i++) {
                intTemp += ((quint8)temp.at(i) << j*8);
                j--;
            }
            amount = intTemp/1000;
            if (name != "") {
                if (amount != 0) {
                    priceSum += amount * price;
                    m_common->sqlHelper()->decreaseItem(barCode, amount, price, QDateTime(date));
                    qWarning() << name << amount << barCode;
                }
            }
        }
        db.commit();
        file.close();
        emit fileReaded(filename, priceSum);
        return true;
    }
    emit fileReadFailed(filename);
    return false;
}


bool ECRduFileReader::readFile(QString filename, QList<ECRDuFileData> *list)
{
    list->clear();
    QFile file(filename);
    if (file.open(QFile::ReadOnly)) {
        /**
         * The data file first 0x100 byte is a header,
         * the PLUs data follows it in the following format:
         * 0x00:0x06    (7 bytes):  barcode in gray format
         * 0x07:0x16    (16 bytes): item name
         * 0x17         (1 byte):   0x00 (terminating \0 character to the string?)
         * 0x18:0x1B    (4 byte):   price in int32 format (big endian)
         * 0x1C:0x20    (4 byte):   amount*1000 in int32 format (big endian)
         */
        if (!file.seek(0x100)) {
            emit fileReadFailed(filename);
            return false;
        }

        while (!file.atEnd()) {
            QByteArray temp = file.read(32);
            QString barCode, name;
            quint32 price = 0;
            ECRDuFileData data;

            quint32 sum = 0;
            for (int i = 0x00; i<0x07; i++) {
                sum += (quint8)temp.at(i);
                if (sum) {
                    if (barCode.isEmpty())
                        barCode = barCode.append(QString::number((quint8)temp.at(i), 16));
                    else
                        barCode = barCode.append(QString::number((quint8)temp.at(i), 16).rightJustified(2, '0'));
                }
            }
            data.barcode = barCode;

            for (int i = 0x07; i<0x17; i++) {
                if (temp.at(i) != 0)
                    name = name.append(QChar(temp.at(i)));
            }
            data.name = name;

            int intTemp = 0, j = 2;
            for (int i = 0x18; i<0x1B; i++) {
                price += ((quint8)temp.at(i) << j*8);
                j--;
            }
            data.price = price;

            j = 3;
            for (int i = 0x1C; i<0x20; i++) {
                intTemp += ((quint8)temp.at(i) << j*8);
                j--;
            }
            data.amount = intTemp/1000;
            if (name != "" && data.amount != 0) {
                list->append(data);
            }
        }
        file.close();
        return true;
    }
    return false;
}
