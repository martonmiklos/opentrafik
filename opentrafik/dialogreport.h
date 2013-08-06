#ifndef DIALOGREPORT_H
#define DIALOGREPORT_H

#include <QDialog>
#include <QCalendarWidget>
#include <QSqlQueryModel>

#include "common.h"

namespace Ui {
class DialogReport;
}

class DialogReport : public QDialog
{
    Q_OBJECT
    enum DateSelect {
        StartDate,
        EndDate
    };

public:
    explicit DialogReport(Common *common, QWidget *parent = 0);
    ~DialogReport();
private:
    void generateReport();

public slots:
    void databaseConnected();

private slots:
    void dateSelected(QDate date);
    void on_comboBoxReportInterval_currentIndexChanged(int index);
    void on_pushButtonDate_clicked();
    void on_comboBoxType_currentIndexChanged(int index);

private:
    Ui::DialogReport *ui;
    Common *m_common;
    QStringList reportNames;
    QCalendarWidget *m_calendar;
    DateSelect m_dateSelect;
    QDate m_reportStartDate, m_reportEndDate;
    QSqlQueryModel *m_model;
};

#endif // DIALOGREPORT_H
