#include "statisticflight.h"
#include "qsqlrecord.h"
#include "ui_statisticflight.h"

StatisticFlight::StatisticFlight(DataBase *db, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatisticFlight)
{
    ui->setupUi(this);
    dbStatist = db;

    statisticFlight = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);
    statisticFlight->setBrush(QColor(0, 168, 140));

    connect(dbStatist, &DataBase::sig_SendAnswerNameAeroport, this, &StatisticFlight::ReceiverAnswerNameAiroport);
    connect(dbStatist, &DataBase::sig_SendAnswerStaticYear, this, &StatisticFlight::ViewStaticYear);
    connect(dbStatist, &DataBase::sig_SendAnswerStaticDay, this, &StatisticFlight::ViewStaticDay);
    connect(ui->cb_listMonth, &QComboBox::currentTextChanged, this, &StatisticFlight::RequestStaticDay);
    connect(ui->cb_listNameAiroport, &QComboBox::currentTextChanged, this, &StatisticFlight::RequestStaticDay);

    listMonth << "Январь" << "Февраль" << "Март" << "Апрель" << "Май" << "Июнь" << "Июль" << "Август" << "Сентябрь" << "Октябрь"
              << "Ноябрь" << "Декабрь";
}

StatisticFlight::~StatisticFlight()
{
    delete ui;
    delete dbStatist;
}

void StatisticFlight::initStatistic()
{
    dbStatist->SendRequest(requestNameAiroport);
    ui->cb_listMonth->addItems(listMonth);
    this->show();
}

void StatisticFlight::on_pb_close_clicked()
{
    emit sig_close();
    this->close();
}

void StatisticFlight::ReceiverAnswerNameAiroport(QSqlQueryModel *queryModelNameAiroport)
{
    ui->cb_listNameAiroport->setModel(queryModelNameAiroport);
    ui->cb_listNameAiroport->setModelColumn(0);
}

void StatisticFlight::ViewStaticYear(QSqlQueryModel *answerStatic)
{
    int count = 1;

    xDate.clear();
    yValue.clear();
    xList.clear();

    for (int i = 0; i < answerStatic->rowCount(); ++i) {
        QModelIndex idxFlight = answerStatic->index(i, 0);
        QModelIndex idxDate = answerStatic->index(i, 1);
        yValue.append(answerStatic->data(idxFlight, Qt::DisplayRole).toDouble());
        xDate.append(count++);
        QVariant monthInt = answerStatic->data(idxDate, Qt::DisplayRole).toDate().month();
        QVariant yearInt = answerStatic->data(idxDate, Qt::DisplayRole).toDate().year();
        xList.append(listMonth[monthInt.toInt() - 1] + " . " + yearInt.toString());
    }

    double maxYear = static_cast<double>(*std::max_element(std::begin(yValue), std::end(yValue)));
    MakeGraphStatist(maxYear);
}

void StatisticFlight::ViewStaticDay(QSqlQueryModel *answerStatic)
{
    int selectMonth = (ui->cb_listMonth->currentIndex()) + 1;
    int count = 0;

    xDate.clear();
    yValue.clear();
    ticks.clear();
    xList.clear();

    for (int i = 0; i < answerStatic->rowCount(); ++i) {
        QSqlRecord tempDate = answerStatic->record(i);
        QDate dateDateTemp = tempDate.value("date_trunc").toDate();
        int dateIntTemp = dateDateTemp.month();
        if (selectMonth == dateIntTemp) {
            QModelIndex idxFlight = answerStatic->index(i, 0);
            QModelIndex idxDate = answerStatic->index(i, 1);
            yValue.append(answerStatic->data(idxFlight, Qt::DisplayRole).toDouble());
            xDate.append(count++);
            xList.append(answerStatic->data(idxDate, Qt::DisplayRole).toDate().toString());
        }
    }

    MakeGraphStatist(25);
}

void StatisticFlight::MakeGraphStatist(double maxValue)
{

    ui->customPlot->yAxis->setRange(0, maxValue + 25);

    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(xDate, xList);
    ui->customPlot->xAxis->setTicker(textTicker);
    ui->customPlot->xAxis->setTickLabelRotation(60);

    statisticFlight->setData(xDate, yValue);
    ui->customPlot->rescaleAxes();
    ui->customPlot->replot();
}

void StatisticFlight::closeEvent(QCloseEvent *bar)
{
    emit sig_close();
}

void StatisticFlight::on_pb_StaticYear_clicked()
{
    int row = ui->cb_listNameAiroport->currentIndex();
    QModelIndex idx = ui->cb_listNameAiroport->model()->index(row, 1);
    QString codeAiroport = ui->cb_listNameAiroport->model()->data(idx, Qt::DisplayRole).toString();
    QString requestYearTemp = requestStatisticYearOne + codeAiroport + requestStatisticYearTwo + codeAiroport + requestStatisticYearThree;
    dbStatist->SendRequestStaticYear(requestYearTemp);
}

void StatisticFlight::RequestStaticDay()
{
    int row = ui->cb_listNameAiroport->currentIndex();
    QModelIndex idx = ui->cb_listNameAiroport->model()->index(row, 1);
    QString codeAiroport = ui->cb_listNameAiroport->model()->data(idx, Qt::DisplayRole).toString();
    QString requestDayTemp = requestStatisticDayOne + codeAiroport + requestStatisticDayTwo + codeAiroport + requestStatisticDayThree;
    dbStatist->SendRequestStaticDay(requestDayTemp);
}
