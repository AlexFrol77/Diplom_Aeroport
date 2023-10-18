#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QtConcurrent>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QTableView>
#include <QSqlRecord>
#include <QTimer>
#include <QDebug>
#include <QDateEdit>
#include <QtWidgets>
#include <windows.h>
#include "database.h"
#include "statisticflight.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:

    void ReceivStatusConnect(bool);
    void ReceiveAnswerNameAiroport(QSqlQueryModel *queryModel);
    void ReceiveAnswerFlight(QSqlQueryModel *queryModelFlight);
    void ReceiverAnswerStatisticMonth(QSqlQueryModel *queryModelStatisticMonth);
    void enableWindows();

signals:


private slots:

    void initConnect();
    void EnableButtonQueryFlight();
    void on_pb_receiverFlight_clicked();
    void on_pb_receive_clicked();
    void on_actionStatistica_triggered();
    void on_actionAbout_triggered();
    void on_pb_connect_clicked();
    void on_pb_disconnect_clicked();
    void closeConnect();

private:
    Ui::MainWindow *ui;

    DataBase* db;
    StatisticFlight* staticFlight;

    QTimer* timer;

    void changeComboBox();
    QString codeAiroport;
    bool flagFlight;

    QString request = "SELECT airport_name->>'ru', airport_code FROM bookings.airports_data";
    QString requestArrival = "SELECT flight_no, scheduled_arrival, ad.airport_name->>'ru' from"
                             " bookings.flights f"
                             " JOIN bookings.airports_data ad on ad.airport_code = f.departure_airport"
                             " where f.arrival_airport = ";
    QString requestDeparture = "SELECT flight_no, scheduled_departure, ad.airport_name->>'ru' from"
                               " bookings.flights f"
                               " JOIN bookings.airports_data ad on ad.airport_code = f.arrival_airport"
                               " where f.departure_airport = ";

};
#endif // MAINWINDOW_H
