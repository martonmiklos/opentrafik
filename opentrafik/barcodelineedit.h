#ifndef BARCODELINEEDIT_H
#define BARCODELINEEDIT_H

#include <QLineEdit>
#include <QKeyEvent>

class BarcodeLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit BarcodeLineEdit(QWidget *parent = 0);
    void keyPressEvent(QKeyEvent *event);
signals:
    
public slots:
    
};

#endif // BARCODELINEEDIT_H
