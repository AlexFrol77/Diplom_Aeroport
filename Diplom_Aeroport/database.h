#ifndef DATABASE_H
#define DATABASE_H

#define NAME_DB_MAIN "myDB_Main_Airoport"
#define DRIVER_DB "QPSQL"

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QDate>
#include <QTableView>
#include <QSqlError>
#include <QDebug>

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = nullptr);
    ~DataBase();

    void QueryConnect();

    void SendRequest(QString request);
    void SendRequestFlight(QString tempRequest);
    void SendRequestStaticYear(QString requestStaticYear);
    void SendRequestStaticDay(QString requestStaticDay);
    void Disconnect(QString nameDb);

signals:

    void sig_SendStatusConnect(bool);
    void sig_CloseConnect();
    void sig_SendAnswerNameAeroport(QSqlQueryModel *queryModel);
    void sig_SendAnswerFlight(QSqlQueryModel *queryModelFlight);
    void sig_SendAnswerStaticYear(QSqlQueryModel *queryModelStatic);
    void sig_SendAnswerStaticDay(QSqlQueryModel *queryModelStatic);

private:

    QSqlDatabase* dataBase;
    QSqlQueryModel* queryModel;
    QSqlQueryModel* queryModelFlight;
    QSqlQueryModel* queryModelStatic;

};

#endif // DATABASE_H
