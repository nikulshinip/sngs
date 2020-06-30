#ifndef DBADAPTER_H
#define DBADAPTER_H

#include <QObject>
#include <QVariant>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QSqlError>

class DbAdapter : public QObject
{
    Q_OBJECT
public:
    DbAdapter(QObject *parent = nullptr);
    ~DbAdapter();

    //! функция подулючения к базе данных
    //! path - путь к файлу бд
    bool connectDb(QString path);

    //! возвращает модель списка таблиц в бд
    QSqlQueryModel *getTableNamesModel()const;

    //! возвращает список групп в таблице бд
    //! tableName - имя таблицы в бд
    QList<int> groupsFromTables(const QString &tableName)const;

    //! возвращает модель данных
    QSqlQueryModel *getDataModel()const;

    //! возращает QMap<Time, Value> для группы из таблицы
    //! tableName - имя таблицы
    //! group - имя группы
    static QMap<qreal, qreal> getData(const QString &tableName, const int &group);

public slots:
    //! меняет выбраную таблицу
    bool setTableIntoDataModel(const QString &tableName);

signals:
    //! подключились к бд
    void connectedToDb();

private:
    QSqlDatabase db;
    QSqlQueryModel *tableNamesModel;
    QSqlQueryModel *dataModel;
};

#endif // DBADAPTER_H
