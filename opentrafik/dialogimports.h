#ifndef DIALOGIMPORTS_H
#define DIALOGIMPORTS_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QShowEvent>

#include "common.h"

namespace Ui {
class DialogImports;
}

class DialogImports : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogImports(Common *common, QWidget *parent = 0);
    ~DialogImports();
    void showEvent(QShowEvent *event);

public slots:
    void databaseConnected();
    
private slots:
    void on_tableViewImports_doubleClicked(const QModelIndex &index);

    void on_pushButtonImport_clicked();

    void on_pushButtonSubstracImports_clicked();

private:
    Ui::DialogImports *ui;
    void reloadModel();
    QSqlQueryModel *m_model;
    Common *m_common;
};

#endif // DIALOGIMPORTS_H
