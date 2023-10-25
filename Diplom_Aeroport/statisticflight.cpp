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
    graph = new QCPGraph(ui->customPlot->xAxis, ui->customPlot->yAxis);
    graph->setBrush(QColor(0, 168, 140, 150));
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

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

    if (!yValue.isEmpty()) {
        maxYear = static_cast<double>(*std::max_element(std::begin(yValue), std::end(yValue)));
    }
    MakeGraphStatist(maxYear, flagGraph = false);
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

    if (!yValue.isEmpty()) {
        maxDay = *std::max_element(std::begin(yValue), std::end(yValue));
    }
    MakeGraphStatist(maxDay, flagGraph = true);
}

void StatisticFlight::MakeGraphStatist(double maxValue, bool flagGraph)
{

    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText); 

    double lastValueDate = 0;
    if (!yValue.isEmpty()) {
        lastValueDate = xDate.last();
    }

    ui->customPlot->clearItems();

    if (flagGraph) {
        graph->setData(xDate, yValue);
        graph->setVisible(flagGraph);
        statisticFlight->setVisible(!flagGraph);
        ui->customPlot->xAxis->setRange(-0.5, lastValueDate + 1);
    }
    if (!flagGraph) {
        statisticFlight->setData(xDate, yValue);
        statisticFlight->setVisible(!flagGraph);
        graph->setVisible(flagGraph);
        ui->customPlot->xAxis->setRange(0, lastValueDate + 1);
    }

    for (int i = 0; i < xDate.size(); i++) {
        QCPItemText *textLabel = new QCPItemText(ui->customPlot);
        textLabel->setText(QString::number(yValue[i]));
        textLabel->position->setType(QCPItemPosition::ptPlotCoords);
        textLabel->position->setCoords(xDate[i], yValue[i]);
        textLabel->setFont(QFont(font().family(), 12));
        QPen* penMax = new QPen;
        penMax->setWidth(1);
        penMax->setColor(Qt::red);
        penMax->setStyle(Qt::SolidLine);
        textLabel->setPen(*penMax);
        textLabel->setBrush(QBrush(Qt::white));
    }

    textTicker->addTicks(xDate, xList);
    ui->customPlot->xAxis->setTicker(textTicker);
    ui->customPlot->xAxis->setTickLabelRotation(60);

    ui->customPlot->yAxis->setRange(0, maxValue + 25);;
    ui->customPlot->replot();
}

void StatisticFlight::closeEvent(QCloseEvent *bar)
{
    emit sig_close();
}

QString StatisticFlight::formingRequest(QString calendar)
{
    int row = ui->cb_listNameAiroport->currentIndex();
    QModelIndex idx = ui->cb_listNameAiroport->model()->index(row, 1);
    QString codeAiroport = ui->cb_listNameAiroport->model()->data(idx, Qt::DisplayRole).toString();
    QString requestTemp = requestStatisticOneCalendar
                          + calendar
                          + requestStatisticTwoAiroport
                          + codeAiroport
                          + requestStatisticThreeAiroport
                          + codeAiroport
                          + requestStatisticFourCalendar
                          + calendar
                          + requestStatisticFiveEnd;
    return requestTemp;
}

void StatisticFlight::on_pb_StaticYear_clicked()
{
    dbStatist->SendRequestStaticYear(formingRequest("month"));
}

void StatisticFlight::RequestStaticDay()
{
    dbStatist->SendRequestStaticDay(formingRequest("day"));
}
