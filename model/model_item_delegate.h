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

    void usuall(QPainter* painter, const QStyleOptionViewItem& option,
               const QModelIndex& index) const;

    void progress(QPainter* painter, const QStyleOptionViewItem& option,
               const QModelIndex& index, int progressPercentage) const;

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

private:
    static void drawCheckbox(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index);
    static QPainterPath roundRectPath(const QRect &rect, int offset);
    static QLinearGradient buildGradient(QRect rect, QColor color, bool dark);
    static QLinearGradient defaultState(QRect rect, bool dark);
    static QLinearGradient listenedState(QRect rect, bool dark);
    static QLinearGradient likedState(QRect rect, bool dark);
    static QLinearGradient playedState(QRect rect, bool dark);
    static QLinearGradient unprocessedState(QRect rect, bool dark);
};

#endif // MODEL_ITEM_DELEGATE_H
