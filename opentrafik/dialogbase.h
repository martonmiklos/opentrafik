#ifndef DIALOGBASE_H
#define DIALOGBASE_H

#include <QDialog>
#include <QSqlQueryModel>

#include "common.h"

class DialogBase : public QDialog
{
    Q_OBJECT
public:
    DialogBase(Common *common, QWidget *parent = 0);

public slots:
    virtual void databaseConnected() = 0;

protected:
    Common *m_common;
};

class DialogBaseWithModel : public DialogBase
{
    Q_OBJECT
public:
    struct SQLCondition {
        QString field, condition;
    };

    DialogBaseWithModel(Common *common, QWidget *parent = 0);

    /*QString conditions();
    void addCondition(SQLCondition condition);
    void clearConditions();
    void removeCondition(QString field);*/

protected:
    QSqlQueryModel *m_model;

    //void reloadModel();

private:
    QList<SQLCondition> m_conditions;
};

#endif // DIALOGBASE_H
