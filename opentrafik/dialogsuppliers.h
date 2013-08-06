#ifndef DIALOGSUPPLIERS_H
#define DIALOGSUPPLIERS_H

#include "dialogbase.h"


namespace Ui {
class DialogSuppliers;
}

class DialogSuppliers : public DialogBaseWithModel
{
    Q_OBJECT
    
public:
    explicit DialogSuppliers(Common *common, QWidget *parent = 0);
    ~DialogSuppliers();
    void databaseConnected();
    void showEvent(QShowEvent *);
    void connectDataChanged();

private slots:
    void on_pushButtonSave_clicked();
    void on_pushButtonNew_clicked();
    void on_tableView_activated(const QModelIndex &index);
    void dataChanged();

private:
    Ui::DialogSuppliers *ui;
    void reloadModel();
    qlonglong m_currentEditId;
    bool m_modified;

    bool loadNewAllowed();
};

#endif // DIALOGSUPPLIERS_H
