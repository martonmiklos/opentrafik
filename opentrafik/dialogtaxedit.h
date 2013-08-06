#ifndef DIALOGTAXEDIT_H
#define DIALOGTAXEDIT_H

#include <QDialog>
#include "common.h"

namespace Ui {
class DialogTaxEdit;
}

class DialogTaxEdit : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogTaxEdit(Common *common, QWidget *parent = 0);
    ~DialogTaxEdit();
    
private slots:
    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::DialogTaxEdit *ui;
    Common *m_common;
};

#endif // DIALOGTAXEDIT_H
