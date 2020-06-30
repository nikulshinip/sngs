#include "grouplistdelegate.h"

GroupListDelegate::GroupListDelegate(QObject *parent) : QStyledItemDelegate(parent){

}

QWidget *GroupListDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const{
    Q_UNUSED(option);
    QWidget* editor = nullptr;
    if(index.column() == 0){
        ColorEditor* colorEditor = new ColorEditor(parent);
        colorEditor->setColor(index.data(Qt::DecorationRole).value<QColor>());
        colorEditor->display();
        editor = colorEditor;
    }
    return editor;
}

void GroupListDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const{
    if(index.column() == 0){
        ColorEditor* colorEditor = static_cast<ColorEditor*>(editor);
        model->setData(index, colorEditor->getColor(), Qt::DecorationRole);
    }
}

//void groupListDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const{
//    Q_UNUSED(index);
//    editor->setGeometry(option.rect);
//}
