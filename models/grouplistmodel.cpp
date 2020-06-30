#include "grouplistmodel.h"

GroupListModel::GroupListModel(DbAdapter *dbAdapter, QObject *parent)
    : QAbstractListModel(parent), dbAdapter(dbAdapter)
{
}

//=================================================================================================
int GroupListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return groups.count();
}

//=================================================================================================
Qt::ItemFlags GroupListModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsUserCheckable;
}

//=================================================================================================
QVariant GroupListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    switch (role) {
        case Qt::DisplayRole    : return groups.at(index.row());
        case Qt::CheckStateRole : return checkeds.at(index.row()) ? Qt::Checked : Qt::Unchecked;
        case Qt::DecorationRole : return colors.at(index.row());
        default : return QVariant();
    }
}

//-------------------------------------------------------------------------------------------------
bool GroupListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool ok = false;
    if (role == Qt::CheckStateRole){
        checkeds[index.row()] = value.toBool();
        emit changed(index.row());
        ok = true;
    }
    if(role == Qt::DecorationRole){
        colors[index.row()] = value.value<QColor>();
        emit changed(index.row());
        ok = true;
    }

    if(ok){
        emit dataChanged(index,index,QVector<int>() << role);
    }
    return ok;
}

//=================================================================================================
QList<int> GroupListModel::groupList() const
{
    return groups;
}

//-------------------------------------------------------------------------------------------------
QList<bool> GroupListModel::checkedsList() const
{
    return checkeds;
}

//-------------------------------------------------------------------------------------------------
void GroupListModel::setAllCheckState(bool state)
{
    for(int i = 0; i < rowCount(); ++i){
        setData(index(i), state, Qt::CheckStateRole);
    }
}

Qt::CheckState GroupListModel::checksState() const
{
    bool allChecked = true;
    bool allUnchecked = true;
    for(int i = 0; i < rowCount(); ++i){
        if(data(index(i), Qt::CheckStateRole).toBool())
            allUnchecked = false;
        else
            allChecked = false;
    }
    if(allChecked)
        return Qt::Checked;
    if(allUnchecked)
        return Qt::Unchecked;
    return Qt::PartiallyChecked;
}

//-------------------------------------------------------------------------------------------------
QList<QColor> GroupListModel::colorsList() const
{
    return colors;
}

//-------------------------------------------------------------------------------------------------
QString GroupListModel::table() const
{
    return tableName;
}

//=================================================================================================
void GroupListModel::setTable(const QString &tableName)
{
    beginResetModel();
    groups.clear();
    checkeds.clear();
    colors.clear();
    this->tableName = tableName;
    groups = dbAdapter->groupsFromTables(tableName);

    for(int i = 0; i < groups.count(); ++i){
        checkeds += true;
        colors   += QColor(rand() % 256, rand() % 256, rand() % 256);
    }
    endResetModel();
}
