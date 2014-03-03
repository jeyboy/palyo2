#ifndef MODEL_ITEM_DELEGATE_H
#define MODEL_ITEM_DELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QStyleOption>
#include <QLinearGradient>
#include "model_item.h"

class ModelItemDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit ModelItemDelegate(QObject *parent = 0);

//    QWidget *createEditor(QWidget *parent,
//                          const QStyleOptionViewItem &option,
//                          const QModelIndex &index) const;

//    void setEditorData(QWidget *editor, const QModelIndex &index) const;
//    void setModelData(QWidget *editor,
//                      QAbstractItemModel *model,
//                      const QModelIndex &index) const;

//    void updateEditorGeometry(QWidget *editor,
//                              const QStyleOptionViewItem &option,
//                              const QModelIndex &index) const;

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

private:
    static const int offset = 20;
    static const int padding = 5;

    static QPainterPath roundRectPath(const QRect &rect);
    static QLinearGradient defaultState(QRect rect, bool dark);
    static QLinearGradient listenedState(QRect rect, bool dark);
    static QLinearGradient likedState(QRect rect, bool dark);
    static QLinearGradient playedState(QRect rect, bool dark);

};

#endif // MODEL_ITEM_DELEGATE_H
