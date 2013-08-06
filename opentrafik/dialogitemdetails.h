#ifndef DIALOGITEMDETAILS_H
#define DIALOGITEMDETAILS_H

#include <QDialog>
#include <QString>
#include "common.h"

namespace Ui {
class DialogItemDetails;
}

class DialogItemDetails : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogItemDetails(Common *common, QWidget *parent = 0);
    ~DialogItemDetails();
    void showEvent(QShowEvent *event);

private slots:
    void on_lineEditBarcode_returnPressed();

private:
    Ui::DialogItemDetails *ui;
    Common *m_common;
    void clearData();
};

#endif // DIALOGITEMDETAILS_H
