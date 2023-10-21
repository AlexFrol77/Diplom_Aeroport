#ifndef STATISTICFLIGHT_H
#define STATISTICFLIGHT_H

#include <QWidget>
#include <QSqlDatabase>
#include <QtConcurrent>
#include <QSqlQueryModel>
#include <QDebug>
#include "database.h"
#include "D:\Netology_QT_Project\Diplom_Aeroport\Diplom_Aeroport\qcustomplot.h"


namespace Ui {
class StatisticFlight;
}

class StatisticFlight : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticFlight(DataBase *db, QWidget *parent = nullptr);
    ~StatisticFlight();

    void initStatistic();


signals:

    void sig_close();

private slots:

    void on_pb_close_clicked();
    void ReceiverAnswerNameAiroport(QSqlQueryModel *queryModelNameAiroport);
    void ViewStaticYear(QSqlQueryModel *answerStatic);
    void ViewStaticDay(QSqlQueryModel *answerStatic);
    void RequestStaticDay();
    void on_pb_StaticYear_clicked();

private:
    Ui::StatisticFlight *ui;

    DataBase* dbStatist;

    QCPBars *statisticFlight;


    QVector<double> ticks;

    QVector<double> yValue;
    QVector<double> xDate;
    QVector<QString> xList;
    QVector<QString> listMonth;

    void MakeGraphStatist(double maxValue);

    void closeEvent(QCloseEvent *bar);

    QString requestNameAiroport = "SELECT airport_name->>'ru', airport_code FROM bookings.airports_data";
    QString requestStatisticYearOne = "SELECT count(flight_no), date_trunc('month', scheduled_departure) from"
                                    " bookings.flights f"
                                    " WHERE (scheduled_departure::date > date('2016-08-31') and"
                                    " scheduled_departure::date <= date('2017-08-31')) and"
                                    " ( departure_airport = '";
    QString requestStatisticYearTwo = " ' or arrival_airport = '";
    QString requestStatisticYearThree = " ' ) GROUP BY date_trunc('month', scheduled_departure)";
    QString requestStatisticDayOne = "SELECT count(flight_no), date_trunc('day', scheduled_departure) from"
                                     " bookings.flights f"
                                     " WHERE(scheduled_departure::date > date('2016-08-31') and"
                                     " scheduled_departure::date <= date('2017-08-31')) and ( departure_airport = '";
    QString requestStatisticDayTwo = "' or arrival_airport = '";
    QString requestStatisticDayThree = "') GROUP BY date_trunc('day', scheduled_departure)";
};

#endif // STATISTICFLIGHT_H
