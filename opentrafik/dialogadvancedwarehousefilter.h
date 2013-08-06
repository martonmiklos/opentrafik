#ifndef DIALOGADVANCEDWAREHOUSEFILTER_H
#define DIALOGADVANCEDWAREHOUSEFILTER_H

#include <QDialog>

namespace Ui {
class DialogAdvancedWarehouseFilter;
}

class DialogAdvancedWarehouseFilter : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogAdvancedWarehouseFilter(QWidget *parent = 0);
    ~DialogAdvancedWarehouseFilter();
    
private slots:
    void on_checkBox_toggled(bool checked);

signals:
    void filterChanged(QString);

private:
    Ui::DialogAdvancedWarehouseFilter *ui;
    QStringList m_filterStrings;
};

#endif // DIALOGADVANCEDWAREHOUSEFILTER_H
