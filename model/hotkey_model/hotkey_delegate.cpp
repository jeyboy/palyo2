#include "hotkey_delegate.h"

void HotkeyDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyledItemDelegate::paint(painter, option, index);
}

QSize HotkeyDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    return QStyledItemDelegate::sizeHint(option, index);
}

QWidget * HotkeyDelegate::createEditor(QWidget *parent,
                                     const QStyleOptionViewItem &,
                                     const QModelIndex & index) const {
//    if (index.data().canConvert<StarRating>()) {
//        StarEditor *editor = new StarEditor(parent);
//        connect(editor, SIGNAL(editingFinished()),
//                 this, SLOT(commitAndCloseEditor()));
//        return editor;
//    } else {
//        return QStyledItemDelegate::createEditor(parent, option, index);
//    }

//    QWidget * editor = QStyledItemDelegate::createEditor(parent, option, index);

    HotkeyEditor * editor = new HotkeyEditor(index.data(Qt::UserRole).toInt(), parent);
    connect(editor, SIGNAL(editingFinished()), this, SLOT(commitAndCloseEditor()));

    return editor;
}

void HotkeyDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    QStyledItemDelegate::setEditorData(editor, index);
}

void HotkeyDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    QStyledItemDelegate::setModelData(editor, model, index);

//    if (index.data().canConvert<StarRating>()) {
//        StarEditor *starEditor = qobject_cast<StarEditor *>(editor);
//        model->setData(index, QVariant::fromValue(starEditor->starRating()));
//    } else {
//        QStyledItemDelegate::setModelData(editor, model, index);
//    }
}

void HotkeyDelegate::commitAndCloseEditor() {
//    StarEditor *editor = qobject_cast<StarEditor *>(sender());
    QWidget * editor = qobject_cast<QWidget *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}
