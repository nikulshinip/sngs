#include "dbadapter.h"

DbAdapter::DbAdapter(QObject *parent) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    tableNamesModel = new QSqlQueryModel(this);
    dataModel       = new QSqlQueryModel(this);
}

//-------------------------------------------------------------------------------------------------
//функция подулючения к базе данных
bool DbAdapter::connectDb(QString path)
{
    if(db.open())
        db.close();
    db.setDatabaseName(path);

    if(!db.open())
        return false;

    tableNamesModel->setQuery("SELECT tbl_name "
                              "FROM sqlite_master "
                              "WHERE type = 'table';");

    if(tableNamesModel->lastError().isValid())
        return false;

    return true;
}

//-------------------------------------------------------------------------------------------------
//возвращает модель списка таблиц в бд
QSqlQueryModel *DbAdapter::getTableNamesModel() const
{
    return tableNamesModel;
}

//=================================================================================================
//возвращает список групп в таблице бд
QList<int> DbAdapter::groupsFromTables(const QString &tableName) const
{
    QList<int> out;
    QString queryStr = QString("SELECT Number FROM %1 GROUP BY Number;").arg(tableName);

    QSqlQuery query;
    query.prepare(queryStr);

    if(query.exec()){
        int records = query.record().indexOf("Number");
        while (query.next()) {
            out += query.value(records).toInt();
        }
    }
    return out;
}

//=================================================================================================
//возвращает модель данных
QSqlQueryModel *DbAdapter::getDataModel() const
{
    return dataModel;
}

//=================================================================================================
//возращает QMap<Time, Value> для группы из таблицы
QMap<qreal, qreal> DbAdapter::getData(const QString &tableName, const int &group)
{
    QMap<qreal, qreal> out;
    QString queryStr = QString("SELECT Time, "
                                      "Value "
                               "FROM %1 "
                               "WHERE Number = %2 "
                               "ORDER BY Time ASC;").arg(tableName)
                                                    .arg(group);
    QSqlQuery query;
    query.prepare(queryStr);

    if(query.exec()){
        QSqlRecord records = query.record();
        while (query.next()) {
            out[query.value(records.indexOf("Time")).toInt()] = query.value(records.indexOf("Value")).toReal();
        }
    }
    return out;
}

//-------------------------------------------------------------------------------------------------
//меняет выбраную таблицу
bool DbAdapter::setTableIntoDataModel(const QString &tableName)
{
    if(tableName.isEmpty())
        return false;
    dataModel->setQuery(QString("SELECT datetime(Time, 'unixepoch') AS Время, "
                                                            "Value  AS Значение, "
                                                            "Number AS Группа "
                                "FROM %1").arg(tableName));
    if(dataModel->lastError().isValid())
        return false;
    return true;
}

//=================================================================================================
DbAdapter::~DbAdapter()
{
    if(db.open())
        db.close();
}
