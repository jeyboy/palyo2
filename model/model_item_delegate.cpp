#include "model_item_delegate.h"
#include <QDebug>

ModelItemDelegate::ModelItemDelegate(QObject* parent)
    : QStyledItemDelegate(parent) {
}

QLinearGradient ModelItemDelegate::buildGradient(QRect rect, QColor color, bool dark) {
    QLinearGradient grad(rect.left(), rect.top(), rect.left(), rect.bottom());

    grad.setColorAt(0, color);
    if (dark)
        grad.setColorAt(0.8, Qt::black);
    else
        grad.setColorAt(0.8, Qt::white);

    return grad;
}

QLinearGradient ModelItemDelegate::defaultState(QRect rect, bool dark) {
    return buildGradient(rect, QColor(98, 173, 248), dark);
}
QLinearGradient ModelItemDelegate::listenedState(QRect rect, bool dark) {
    return buildGradient(rect, QColor(240, 128, 128), dark);
}
QLinearGradient ModelItemDelegate::likedState(QRect rect, bool dark) {
    return buildGradient(rect, QColor(232, 196, 0), dark);
}
QLinearGradient ModelItemDelegate::playedState(QRect rect, bool dark) {
    return buildGradient(rect, QColor(144, 238, 144), dark);
}

QLinearGradient ModelItemDelegate::unprocessedState(QRect rect, bool dark) {
    QLinearGradient grad(rect.left(), rect.top(), rect.left(), rect.bottom());

    if (dark) {
        grad.setColorAt(0, QColor(128, 128,128, 92));
        grad.setColorAt(0.8, Qt::black);
    } else
        grad.setColorAt(0, Qt::white);

    return grad;
}

//QWidget * ModelItemDelegate::createEditor(QWidget *parent,
//                      const QStyleOptionViewItem &option,
//                      const QModelIndex &index) const {

//}

//void ModelItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {

//}
//void ModelItemDelegate::setModelData(QWidget *editor,
//                  QAbstractItemModel *model,
//                  const QModelIndex &index) const;

//void ModelItemDelegate::updateEditorGeometry(QWidget *editor,
//                          const QStyleOptionViewItem &option,
//                          const QModelIndex &index) const;


void ModelItemDelegate::usuall(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    //  //////////////////// remove focus rect
    //  QStyleOptionViewItem option2 = option;
    //  option2.rect.moveLeft(option2.rect.left() + offset);
    //  option2.state = option.state & (~QStyle::State_HasFocus) & (~QStyle::State_Active) & (~QStyle::State_Selected);
    //  /////////////////////////////////////////////
    //  QStyledItemDelegate::paint(painter, option2, index);


    //  if (index.isValid()) {
    //    painter->setPen(Qt::SolidLine);
    //    painter->setPen(QColor(Qt::lightGray));

    //    painter->drawLine(QLine(option.rect.bottomLeft(), option.rect.bottomRight()));
    //    painter->drawLine(QLine(option.rect.topRight(), option.rect.bottomRight()));
    //  }

        int iconSize = 16;
        int delta = (option.state & QStyle::State_MouseOver) ? 32 : 0;
        QColor semiTransparentWhite(255, 255, 255, 48 + delta);
        QColor semiTransparentBlack(0, 0, 0, 48 - delta);

        int x, y, width, height, offset, padding = 5;
        option.rect.getRect(&x, &y, &width, &height);

        int radius = qMin(width, height) / 1.5;

        painter -> save();
        painter -> setRenderHint(QPainter::Antialiasing, true);
        int background_state = index.data(Qt::UserRole).toInt();

        if (background_state != STATE_UNPROCESSED) {
            offset = iconSize - 6;
        } else {
            offset = 5;
        }

        QPainterPath roundRect = roundRectPath(option.rect, offset);

        bool elem_state = option.state & (QStyle::State_Selected | QStyle::State_HasFocus);

        QLinearGradient fill_color;

        switch (background_state) {
            case STATE_DEFAULT:
                fill_color = defaultState(option.rect, elem_state);
                break;
            case STATE_LISTENED:
                fill_color = listenedState(option.rect, elem_state);
                break;
            case STATE_LIKED:
                fill_color = likedState(option.rect, elem_state);
                break;
            case STATE_PLAYED:
                fill_color = playedState(option.rect, elem_state);
                break;
            default:
                fill_color = unprocessedState(option.rect, elem_state);
                break;
        }

        painter -> fillPath(roundRect, fill_color);

    //  if (option.state & (QStyle::State_Selected | QStyle::State_HasFocus))
    //      painter->fillPath(roundRect, semiTransparentBlack);

        int penWidth;
        if (radius < 10)
            penWidth = 3;
        else if (radius < 20)
            penWidth = 5;
        else
            penWidth = 7;

        QPen topPen(semiTransparentWhite, penWidth);
        QPen bottomPen(semiTransparentBlack, penWidth);

        if (option.state & (QStyle::State_Sunken | QStyle::State_On))
            qSwap(topPen, bottomPen);

        int x1 = x;
        int x2 = x + radius;
        int x3 = x + width - radius;
        int x4 = x + width;

        if (option.direction == Qt::RightToLeft) {
            qSwap(x1, x4);
            qSwap(x2, x3);
        }

        QPolygon topHalf;
        topHalf << QPoint(x1, y)
                << QPoint(x4, y)
                << QPoint(x3, y + radius)
                << QPoint(x2, y + height - radius)
                << QPoint(x1, y + height);

        painter -> setClipPath(roundRect);
        painter -> setClipRegion(topHalf, Qt::IntersectClip);
        painter -> setPen(topPen);
        painter -> drawPath(roundRect);

        QPolygon bottomHalf = topHalf;
        bottomHalf[0] = QPoint(x4, y + height);

        painter -> setClipPath(roundRect);
        painter -> setClipRegion(bottomHalf, Qt::IntersectClip);
        painter -> setPen(bottomPen);
        painter -> drawPath(roundRect);

        painter -> setPen(option.palette.foreground().color());
        painter -> setClipping(false);
        painter -> drawPath(roundRect);

        painter -> restore();

        QVariant vfont = index.data(Qt::FontRole);
        QIcon icon;
        QVariant iconVal = index.model() -> data(index, Qt::DecorationRole);

        if (iconVal.isValid()) {
            QPoint topLeft(x - 7, option.rect.y() + 1);
            QPoint bottomRight(topLeft.rx() + iconSize - 1, option.rect.bottom() - 1);

            icon =  qvariant_cast<QIcon>(iconVal);
    //        icon.paint(painter, QRect(topLeft, bottomRight));

            painter -> drawPixmap(QRect(topLeft, bottomRight), icon.pixmap(QSize(16,16)), QRect(0, 0, 16, 16));
        }

        QPoint topLeft(x + offset + padding, option.rect.y());
        QPoint bottomRight(option.rect.right() - padding, option.rect.bottom());
        QRect rectText(topLeft, bottomRight);
        QString s;

        if (vfont.isValid()) {
            QFont font = vfont.value<QFont>();
            painter -> setFont(font);
            QFontMetrics fmf(font);
            s = fmf.elidedText(index.data().toString(), option.textElideMode, rectText.width());
        } else {
            s = option.fontMetrics.elidedText(index.data().toString(), option.textElideMode, rectText.width());
        }

        if (elem_state)
            painter -> setPen(Qt::white);
        painter -> drawText(rectText, Qt::AlignLeft, s);
        painter -> setPen(Qt::black);
}

void ModelItemDelegate::progress(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index, int progressPercentage) const {
    QStyleOptionProgressBarV2 progressBarOption;
    progressBarOption.rect = QRect(option.rect.x(), option.rect.y() + 5 , option.rect.width(), option.rect.height() / 1.2);
    progressBarOption.text = index.model() -> data(index, Qt::DisplayRole).toString();
    progressBarOption.minimum = 0;
    progressBarOption.maximum = 100;
    progressBarOption.progress = progressPercentage;
//    QPalette pal = progressBarOption.palette;
//    QColor col = QColor(35, 35, 25);
//    pal.setColor(QPalette::Highlight, col); // or QPalette::Window doesnt matter
//    progressBarOption.palette = pal;

//    if(option.state & QStyle::State_Selected) {

//    }

    QApplication::style() -> drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
}


void ModelItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    int progressPercentage = index.model() -> data(index, Qt::UserRole + 1).toInt();

    if (progressPercentage != -1) {
        qDebug() << progressPercentage;
        progress(painter, option, index, progressPercentage);
    } else {
        usuall(painter, option, index);
    }
}

QPainterPath ModelItemDelegate::roundRectPath(const QRect &rect, int offset) {
    int radius = qMin(rect.width(), rect.height()) / 2;
    int diam = radius * 1.5;

    int x1, y1, x2, y2;
    rect.getCoords(&x1, &y1, &x2, &y2);
    x1 += offset;

    QPainterPath path;
    path.moveTo(x2, y1 + radius);
    path.arcTo(QRect(x2 - diam, y1, diam, diam), 0.0, +90.0);
    path.lineTo(x1 + radius, y1);
    path.arcTo(QRect(x1, y1, diam, diam), 90.0, +90.0);
    path.lineTo(x1, y2 - radius);
    path.arcTo(QRect(x1, y2 - diam, diam, diam), 180.0, +90.0);
    path.lineTo(x1 + radius, y2);
    path.arcTo(QRect(x2 - diam, y2 - diam, diam, diam), 270.0, +90.0);
    path.closeSubpath();
    return path;
}
