#include "database.h"

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{
    dataBase = new QSqlDatabase();

    queryModel = new QSqlQueryModel;
    queryModelFlight = new QSqlQueryModel;
    queryModelStatic = new QSqlQueryModel;
    *dataBase = QSqlDatabase::addDatabase(DRIVER_DB, NAME_DB_MAIN);
}

DataBase::~DataBase()
{
    delete dataBase;
    delete queryModel;
    delete queryModelFlight;
    delete queryModelStatic;
}

void DataBase::QueryConnect(QString nameDb) {
    *dataBase = QSqlDatabase::database(nameDb);
    dataBase->setHostName("981757-ca08998.tmweb.ru");
    dataBase->setDatabaseName("demo");
    dataBase->setUserName("netology_usr_cpp");
    dataBase->setPassword("CppNeto3");
    dataBase->setPort(5432);

    bool status;
    status = dataBase->open();
    qDebug() << dataBase->lastError().text();
    emit sig_SendStatusConnect(status);
}

void DataBase::SendRequest(QString request) {

    queryModel->setQuery(request, *dataBase);
    qDebug() << dataBase->lastError().text();
    emit sig_SendAnswerNameAeroport(queryModel);
}

void DataBase::SendRequestFlight(QString tempRequest)
{
    queryModelFlight->setQuery(tempRequest, *dataBase);
    emit sig_SendAnswerFlight(queryModelFlight);
}

void DataBase::SendRequestStaticYear(QString requestStaticYear)
{
    queryModelStatic->setQuery(requestStaticYear, *dataBase);
    emit sig_SendAnswerStaticYear(queryModelStatic);
}

void DataBase::SendRequestStaticDay(QString requestStaticDay)
{
    queryModelStatic->setQuery(requestStaticDay, *dataBase);
    emit sig_SendAnswerStaticDay(queryModelStatic);
}

void DataBase::Disconnect(QString nameDb)
{
    *dataBase = QSqlDatabase::database(nameDb);
    dataBase->close();
    emit sig_CloseConnect();
}
