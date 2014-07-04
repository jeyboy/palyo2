#ifndef TREE_VIEW_STYLE_H
#define TREE_VIEW_STYLE_H

#include <QProxyStyle>

class TreeViewStyle : public QProxyStyle {
public:
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const {
        if (element == QStyle::PE_IndicatorItemViewItemDrop) {
            painter -> save();
            painter -> setRenderHint(QPainter::Antialiasing, true);

            QPalette palette;
//QColor(90,108,217)
            QColor c(palette.link().color());
            QPen pen(c);
            pen.setWidth(2);
            c.setAlpha(50);
            QBrush brush(c);

            painter -> setPen(pen);
            painter -> setBrush(brush);
            if(option -> rect.height() == 0) {
                painter -> drawEllipse(option -> rect.topLeft(), 3, 3);
                painter -> drawLine(QPoint(option -> rect.topLeft().x() + 3, option -> rect.topLeft().y()), option -> rect.topRight());
            } else {
                painter -> drawRoundedRect(option -> rect, 5, 5);
            }
            painter -> restore();
        } else {
            QProxyStyle::drawPrimitive(element, option, painter, widget);
        }
    }
};

#endif // TREE_VIEW_STYLE_H
