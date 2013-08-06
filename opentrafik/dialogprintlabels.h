#ifndef DIALOGPRINTLABELS_H
#define DIALOGPRINTLABELS_H

#include "dialogbase.h"

#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QtPrintSupport/QPrintDialog>


namespace Ui {
class DialogPrintLabels;
}

class PrintItemsModel : public QSqlQueryModel
{
public:
    PrintItemsModel(QObject *parent = 0) : QSqlQueryModel(parent) {}

    QVariant data(const QModelIndex &item, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    int columnCount(const QModelIndex &parent) const;
private:
    QList<bool> m_itemsToPrint;
    const QModelIndex sqlIndex(const QModelIndex & index);
};

class DialogPrintLabels : public DialogBase
{
    Q_OBJECT
    
public:
    explicit DialogPrintLabels(Common *common, QWidget *parent = 0);
    ~DialogPrintLabels();
    void databaseConnected();

private slots:
    void on_pushButtonPreview_clicked();

private:
    Ui::DialogPrintLabels *ui;
    QPrinter printer;
    QSqlQueryModel *m_model;
};

#endif // DIALOGPRINTLABELS_H
