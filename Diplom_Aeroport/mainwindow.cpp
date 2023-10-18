#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    db = new DataBase(this);
    staticFlight = new StatisticFlight();
    timer = new QTimer(this);

    connect(db, &DataBase::sig_SendStatusConnect, this, &MainWindow::ReceivStatusConnect);
    connect(db, &DataBase::sig_SendAnswerNameAeroport, this, &MainWindow::ReceiveAnswerNameAiroport);
    connect(db, &DataBase::sig_SendAnswerFlight, this, &MainWindow::ReceiveAnswerFlight);
    connect(ui->rb_arrival, &QRadioButton::toggled, this, &MainWindow::EnableButtonQueryFlight);
    connect(ui->rb_departure, &QRadioButton::toggled, this, &MainWindow::EnableButtonQueryFlight);
    connect(staticFlight, &StatisticFlight::sig_newConnectDb, this, &MainWindow::enableWindows);
    connect(db, &DataBase::sig_CloseConnect, this, &MainWindow::closeConnect);
    connect(timer, &QTimer::timeout, this, &MainWindow::initConnect);
    ui->pb_receiverFlight->setEnabled(false);
    ui->pb_receive->setEnabled(false);

    initConnect();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete db;
    delete staticFlight;
}

void MainWindow::ReceivStatusConnect(bool status)
{
    if (status) {
        ui->lb_statusConnect->setText("База данных успешно подключена");
        ui->lb_statusConnect->setStyleSheet("color : green");
        db->SendRequest(request);
        ui->rb_arrival->setEnabled(true);
        ui->rb_departure->setEnabled(true);
        ui->pb_receive->setEnabled(true);
    } else {
        ui->lb_statusConnect->setText("Не удалось подключиться к базе данных");
        ui->lb_statusConnect->setStyleSheet("color : red");
        timer->start(5000);
        ui->rb_arrival->setEnabled(false);
        ui->rb_departure->setEnabled(false);
        ui->pb_receiverFlight->setEnabled(false);
        ui->pb_receive->setEnabled(false);
        qDebug() << "Overload connect";
    }
}

void MainWindow::ReceiveAnswerNameAiroport(QSqlQueryModel *queryModel)
{
    ui->cb_listAiroport->setModel(queryModel);
    ui->cb_listAiroport->setModelColumn(0);
}

void MainWindow::ReceiveAnswerFlight(QSqlQueryModel *queryModelFlight)
{
    ui->tv_viewDateBase->setModel(queryModelFlight);
    queryModelFlight->setHeaderData(0, Qt::Horizontal, tr("Номер рейса"));

    if (flagFlight) {
        queryModelFlight->setHeaderData(1, Qt::Horizontal, tr("Дата и время прилёта"));
        queryModelFlight->setHeaderData(2, Qt::Horizontal, tr("Аэропорт отправления"));
    }
    if (!flagFlight) {
        queryModelFlight->setHeaderData(1, Qt::Horizontal, tr("Дата и время вылета"));
        queryModelFlight->setHeaderData(2, Qt::Horizontal, tr("Аэропорт прилета"));
    }
    ui->tv_viewDateBase->resizeRowsToContents();
    ui->tv_viewDateBase->resizeColumnsToContents();
}

void MainWindow::ReceiverAnswerStatisticMonth(QSqlQueryModel *queryModelStatisticMonth)
{
    ui->tv_viewDateBase->setModel(queryModelStatisticMonth);
    ui->tv_viewDateBase->resizeRowsToContents();
    ui->tv_viewDateBase->resizeColumnsToContents();
}

void MainWindow::enableWindows()
{
    this->setEnabled(true);
}

void MainWindow::initConnect()
{
    db->QueryConnect(NAME_DB_MAIN);
}

void MainWindow::EnableButtonQueryFlight()
{
    ui->pb_receiverFlight->setEnabled(true);
}

void MainWindow::on_pb_receiverFlight_clicked()
{
        int row = ui->cb_listAiroport->currentIndex();
        QModelIndex idx = ui->cb_listAiroport->model()->index(row, 1);
        codeAiroport = ui->cb_listAiroport->model()->data(idx, Qt::DisplayRole).toString();
        QDate selectedDate = ui->dE_date->date();
        QString tempRequest;

        if (ui->rb_arrival->isChecked()) {
            flagFlight = true;
            tempRequest = requestArrival + "'" + codeAiroport + "'"
                          + " AND DATE(f.scheduled_arrival) = " + "'"
                          + selectedDate.toString() + "'";
        }
        if (ui->rb_departure->isChecked()) {
            flagFlight = false;
            tempRequest = requestDeparture + "'" + codeAiroport + "'"
                         + " AND DATE(f.scheduled_departure) = " + "'"
                         + selectedDate.toString() + "'";
        }

        db->SendRequestFlight(tempRequest);
}


void MainWindow::on_pb_receive_clicked()
{
    staticFlight->initNameAiroport();
    this->setEnabled(false);
}


void MainWindow::on_actionStatistica_triggered()
{
    staticFlight->initNameAiroport();
    this->setEnabled(false);
}


void MainWindow::on_actionAbout_triggered()
{

    QMessageBox::about(0, "About", "Alex Frol \n" "City Moscow \n");

}


void MainWindow::on_pb_connect_clicked()
{
    initConnect();
}


void MainWindow::on_pb_disconnect_clicked()
{
    db->Disconnect(NAME_DB_MAIN);
}

void MainWindow::closeConnect()
{
    ui->lb_statusConnect->setText("Подключение к базе данных разорвано");
    ui->lb_statusConnect->setStyleSheet("color : red");
    ui->rb_arrival->setEnabled(false);
    ui->rb_departure->setEnabled(false);
    ui->pb_receiverFlight->setEnabled(false);
    ui->pb_receive->setEnabled(false);
}

