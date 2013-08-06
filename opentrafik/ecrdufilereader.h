#ifndef ECRDUFILEREADER_H
#define ECRDUFILEREADER_H

#include <QObject>
#include <QDate>
#include "common.h"

class Common;
struct ECRDuFileData;

class ECRduFileReader : public QObject
{
    Q_OBJECT
public:
    ECRduFileReader(Common *common);

private:
    Common *m_common;

signals:
    void fileReaded(QString path, double price);
    void fileReadFailed(QString path);

public slots:
    bool readFile(QString file, QDate date = QDate());
    bool readFile(QString filename, QList<ECRDuFileData>*data);
};

#endif // ECRDUFILEREADER_H
