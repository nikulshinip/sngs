#ifndef GROUPLISTMODEL_H
#define GROUPLISTMODEL_H

#include <QAbstractListModel>
#include <QColor>
#include "../controllers/dbadapter.h"

//! модель для отображения списка групп(графиков)
//! здесь не очень красиво то, что модель содержит данные, но для этого проекта я решил не создавать
//! отдельные классы данных.
class GroupListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit GroupListModel(DbAdapter *dbAdapter, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    //! возвращает QList название групп(графиков)
    QList<int> groupList()const;
    //! возвращает QList ChekeState
    QList<bool> checkedsList()const;
    //! меняет ChrckState всех групп(графиков)
    void setAllCheckState(bool state);
    //! возвращает общий CheckState
    Qt::CheckState checksState()const;
    //! возвращает QList цветов групп(графиков)
    QList<QColor> colorsList()const;
    //! возвращает название активной таблицы
    QString table()const;

public slots:
    //! Смена таблицы
    void setTable(const QString &tableName);

signals:
    //! произошли изменения в строке
    void changed(int row);

private:
    QList<int> groups;
    QList<bool> checkeds;
    QList<QColor> colors;

    QString tableName{""};

    DbAdapter *dbAdapter;

};

#endif // GROUPLISTMODEL_H
