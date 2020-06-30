#ifndef DATAPROXYMODEL_H
#define DATAPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "grouplistmodel.h"

//! фильтр данных для отображения в QTableView
class DataProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    DataProxyModel(GroupListModel *groupListModel, QObject *parent = nullptr);

protected:
    //! фильтр строк таблицы
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    GroupListModel *groupListModel;
};

#endif // DATAPROXYMODEL_H
