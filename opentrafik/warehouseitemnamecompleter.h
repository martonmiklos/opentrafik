#ifndef WAREHOUSEITEMNAMECOMPLETER_H
#define WAREHOUSEITEMNAMECOMPLETER_H

#include <QCompleter>
#include <QSqlQueryModel>

class WarehouseItemNameCompleter : public QCompleter
{
    Q_OBJECT
public:
    explicit WarehouseItemNameCompleter(QObject *parent = 0);
    void refresh();
private:
    QSqlQueryModel *m_model;
signals:
    
public slots:
    
};

#endif // WAREHOUSEITEMNAMECOMPLETER_H
