#include "dialogreport.h"
#include "ui_dialogreport.h"

#include <QDebug>
#include <QSqlQuery>

DialogReport::DialogReport(Common *common, QWidget *parent) :
    QDialog(parent, Qt::Window),
    ui(new Ui::DialogReport),
    m_common(common)
{
    ui->setupUi(this);
    reportNames << tr("Daily report") << tr("Weekly report")  << tr("Monthly report") << tr("Custom report");
    m_calendar = new QCalendarWidget();
    m_calendar->hide();
    connect(m_calendar, SIGNAL(activated(QDate)), this, SLOT(dateSelected(QDate)));
    m_calendar->resize(480, 320);

    m_model = new QSqlQueryModel(this);
    ui->tableViewResults->setModel(m_model);
}

DialogReport::~DialogReport()
{
    delete m_calendar;
    delete ui;
}

void DialogReport::generateReport()
{
    QString typeCondition;
    QString titleString = tr(" report - ");
    switch (ui->comboBoxType->currentIndex()) {
    case 0: //sell
        titleString = tr(" sell report - ");
        typeCondition = "AND amount_change < 0";
        break;
    case 1: //stocked
        titleString = tr(" stock report - ");
        typeCondition = "AND amount_change > 0";
        break;
    }

    QSqlQuery query;
    query.prepare("SELECT WarehouseItem.name, SUM(WarehouseChange.amount_change) as warehouse_change, SUM(WarehouseChange.amount_change*WarehouseChange.price_brutto_out) as warehouse_price_change "
                  "FROM warehouse_changes AS WarehouseChange "
                  "LEFT JOIN warehouse_items AS WarehouseItem "
                  "ON (WarehouseChange.warehouse_item_id = WarehouseItem.id) "
                  "WHERE (WarehouseChange.date BETWEEN :start AND :end) "+typeCondition+" GROUP BY WarehouseChange.warehouse_item_id "
                  );
    query.bindValue(":start", m_reportStartDate);
    query.bindValue(":end", m_reportEndDate);
    if (query.exec()) {
        m_model->setQuery(query);
        ui->tableViewResults->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
        ui->tableViewResults->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    }

    query.prepare("SELECT SUM(amount_change) as  warehouse_change, SUM(amount_change*price_brutto_out) as warehouse_price_change "
                  "FROM warehouse_changes AS WarehouseChange "
                  "WHERE (WarehouseChange.date BETWEEN :start AND :end) "+typeCondition);
    query.bindValue(":start", m_reportStartDate);
    query.bindValue(":end", m_reportEndDate);
    if (query.exec()) {
        if (query.next()) {
            ui->labelSumPieces->setText(tr("%1 pcs.").arg(query.value(0).toDouble()));
            ui->labelSumPrice->setText(tr("%1 Ft").arg(query.value(1).toDouble()));
        }
    } else {
        qWarning() << query.lastQuery();
    }

    QString dateString = "";
    if (ui->comboBoxReportInterval->currentIndex() == 0) {
        dateString = m_reportStartDate.toString("yyyy-MM-dd");
    } else {
        dateString = m_reportStartDate.toString("yyyy-MM-dd")+" - "+m_reportEndDate.toString("yyyy-MM-dd");
    }

    this->setWindowTitle(ui->comboBoxReportInterval->currentText()+titleString+dateString);

    if (ui->tableViewResults->horizontalHeader()->count() == 3) {
        m_model->setHeaderData(0, Qt::Horizontal, tr("Name"));
        m_model->setHeaderData(1, Qt::Horizontal, tr("Pcs"));
        m_model->setHeaderData(2, Qt::Horizontal, tr("Sell price sum"));
    }
}

void DialogReport::databaseConnected()
{
    dateSelected(QDate::currentDate());
}

void DialogReport::on_comboBoxReportInterval_currentIndexChanged(int index)
{
    setWindowTitle(reportNames.at(index));
}

void DialogReport::dateSelected(QDate date)
{
    m_calendar->close();
    switch (ui->comboBoxReportInterval->currentIndex()) {
    default:
    case 0: // daily
        m_reportStartDate = date;
        m_reportEndDate = date.addDays(1);
        generateReport();
        break;
    case 1: // weekly
        m_reportStartDate = date.addDays(-(date.dayOfWeek()-1));
        m_reportEndDate = date.addDays(7-date.dayOfWeek());
        generateReport();
        break;
    case 2: // monthly
        m_reportStartDate = date.addDays(-(date.day()-1));
        m_reportEndDate = date.addDays(date.daysInMonth() - date.day());
        generateReport();
        break;
    case 3: // custom
        if (m_dateSelect == StartDate) {
            m_reportStartDate = date;
            m_calendar->setWindowTitle(tr("Select the report last day"));
            m_calendar->show();
            m_dateSelect = EndDate;
            return;
        } else {  // endDate
            m_reportEndDate = date;
            generateReport();
        }
        break;
    }
}

void DialogReport::on_pushButtonDate_clicked()
{
    if (ui->comboBoxReportInterval->currentIndex() == 3)
        m_calendar->setWindowTitle(tr("Select the report first day"));
    else
        m_calendar->setWindowTitle(tr("Select the report date"));

    m_calendar->show();
    m_dateSelect = StartDate;
}

void DialogReport::on_comboBoxType_currentIndexChanged(int )
{
    generateReport();
}
