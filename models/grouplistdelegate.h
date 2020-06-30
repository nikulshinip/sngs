#ifndef SENSORTABLEDELEGATE_H
#define SENSORTABLEDELEGATE_H
#include <QStyledItemDelegate>
#include "../widgets/coloreditor.h"

//! Простой делегат, для возможности редактирования светов графиков.
class GroupListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    GroupListDelegate(QObject* parent = nullptr);

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const  override;

//    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

//    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // SENSORTABLEDELEGATE_H
