#ifndef DIALOGIMPORTDETAILS_H
#define DIALOGIMPORTDETAILS_H

#include <QDialog>
#include <QFileDialog>
#include <QSettings>
#include <QAbstractTableModel>

#include "common.h"

namespace Ui {
class DialogImportDetails;
}

// Foward declaration
struct ECRDuFileData;
class Common;

class ECRDUFileModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    ECRDUFileModel(Common *common, QObject *parent = 0);
    bool readFile(QString fileName);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    Qt::ItemFlags flags(const QModelIndex &index) const;

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    double amountChangeSum() const {return m_amountChangeSum;}
    double priceChangeSum() const {return m_priceChangeSum;}
    bool saveToDatabase(QDate date);
    quint32 failedCnt() const {return m_failedCnt;}
private:
    QList<ECRDuFileData> m_data;
    Common *m_common;
    double m_amountChangeSum, m_priceChangeSum;
    quint32 m_failedCnt;
};



class DialogImportFileDetails : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogImportFileDetails(Common *common, QWidget *parent = 0);
    ~DialogImportFileDetails();
    
public slots:
    void importFile(QString fileName);

private slots:
    void on_pushButtonBrowse_clicked();
    void on_pushButtonImport_clicked();

private:
    Ui::DialogImportDetails *ui;
    Common *m_common;
    QSettings m_settings;
    ECRDUFileModel *m_model;
signals:
    void fileReaded();
};


#endif // DIALOGIMPORTDETAILS_H
