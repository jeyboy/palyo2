#ifndef WIDGET_TITLE_H
#define WIDGET_TITLE_H

// NOT USED


#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QStyle>
#include <QMouseEvent>
#include <QApplication>

class Titlebar : public QWidget {
    Q_OBJECT

public:
    //QStyle::SP_TitleBarNormalButton
    //QStyle::SP_TitleBarMinButton
    //QStyle::SP_TitleBarMaxButton
    //QStyle::SP_DockWidgetCloseButton

    Titlebar(QWidget *parent) : parent(parent) {
        QLabel * label = new QLabel(parent -> windowTitle());
        QPushButton *buttonClose = new QPushButton(QApplication::style() -> standardIcon(QStyle::SP_DockWidgetCloseButton, 0, this) ,"", this);

        QHBoxLayout *layout = new QHBoxLayout(this);
        layout -> addWidget(label, 1);
        layout -> addWidget(buttonClose);

        connect(buttonClose, SIGNAL(clicked()), parent, SLOT(close()));
    }

protected:
    void mousePressEvent(QMouseEvent *event) {
        if(event -> button() == Qt::LeftButton)
            cursor = event -> globalPos() - geometry().topLeft();
    }

    void mouseMoveEvent(QMouseEvent *event) {
        if(event -> buttons() & Qt::LeftButton)
            parent -> move(event -> globalPos() - cursor);
    }
private:
    QWidget *parent;
    QPoint cursor;
};

#endif // WIDGET_TITLE_H

//A tabified dockwidget can be set as the selected tab like this:
//dockwidget.raise()

// icons
//QStyle::SP_TitleBarNormalButton
//QStyle::SP_TitleBarMinButton
//QStyle::SP_TitleBarMaxButton
//QStyle::SP_DockWidgetCloseButton


//// connect dockWidget's topLevelChanged signal, which is emitted when its floating property changes, to a user-defined slot
//connect(ui.dockWidget, SIGNAL(topLevelChanged(bool)), this, SLOT(dockWidget_topLevelChanged(bool)));

//// when the floating property of dockWidget is changed from docked to floating
//// we make it a top level window (with minmize, maximize, and close button in the title bar)
//// by calling setWindowFlags(Qt::Window)
//// The dockWidget will automatically regain it's Qt::widget flag when it becomes docked again (by dragging it to the right place or double clicking the title bar)
//void CMainWindow::dockWidget_topLevelChanged(bool isFloating)
//{
//if(isFloating)
//{ ui.dockWidget->setWindowFlags(Qt::Window);
//// setWindowFlags calls setParent() when changing the flags for a window, causing the widget to be hidden.
//// You must call show() to make the widget visible again
//ui.dockWidget->show();
//}
//}
