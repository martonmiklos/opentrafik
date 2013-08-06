#ifndef TAXCOMBOBOX_H
#define TAXCOMBOBOX_H

#include <QComboBox>
#include <QMap>

class TaxComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit TaxComboBox(QWidget *parent = 0);
    ~TaxComboBox();
    void reload();
    double rate(int id = -1);
    void setCurrentId(int id);
    int currentId();

signals:
    
public slots:

private:
    QList<int> m_rates;
};

#endif // TAXCOMBOBOX_H
