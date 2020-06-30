#include "dataproxymodel.h"

DataProxyModel::DataProxyModel(GroupListModel *groupListModel, QObject *parent)
    : QSortFilterProxyModel(parent), groupListModel(groupListModel)
{
    connect(groupListModel, &GroupListModel::dataChanged, this, &DataProxyModel::invalidate);
}

//фильтр строк таблицы
//если в groupListModel стоит галочка, то отображаем эту группу.
bool DataProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent);
    int number = sourceModel()->data(sourceModel()->index(source_row, 2)).toInt();
    int groupNum = groupListModel->groupList().indexOf(number);
    return groupListModel->checkedsList().at(groupNum);
}
