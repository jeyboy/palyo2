/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QScrollArea *left_dock_panel;
    QWidget *scrollAreaWidgetContents;
    QScrollArea *right_dock_panel;
    QWidget *scrollAreaWidgetContents_2;
    QScrollArea *player_panel;
    QWidget *scrollAreaWidgetContents_3;
    QPushButton *pushButton;
    QLCDNumber *lcdNumber;
    QScrollArea *bottom_dock_panel;
    QWidget *scrollAreaWidgetContents_4;
    QScrollArea *top_dock_panel;
    QWidget *scrollAreaWidgetContents_5;
    QTabWidget *tabber;
    QWidget *tab;
    QWidget *tab_2;
    QToolButton *toolButton;
    QToolButton *toolButton_2;
    QToolButton *toolButton_3;
    QToolButton *toolButton_4;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1111, 634);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        left_dock_panel = new QScrollArea(centralWidget);
        left_dock_panel->setObjectName(QStringLiteral("left_dock_panel"));
        left_dock_panel->setGeometry(QRect(0, 80, 40, 481));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(left_dock_panel->sizePolicy().hasHeightForWidth());
        left_dock_panel->setSizePolicy(sizePolicy);
        left_dock_panel->setMinimumSize(QSize(0, 100));
        left_dock_panel->setMaximumSize(QSize(40, 16777215));
        left_dock_panel->setAcceptDrops(true);
        left_dock_panel->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 38, 479));
        left_dock_panel->setWidget(scrollAreaWidgetContents);
        right_dock_panel = new QScrollArea(centralWidget);
        right_dock_panel->setObjectName(QStringLiteral("right_dock_panel"));
        right_dock_panel->setGeometry(QRect(1070, 80, 40, 481));
        sizePolicy.setHeightForWidth(right_dock_panel->sizePolicy().hasHeightForWidth());
        right_dock_panel->setSizePolicy(sizePolicy);
        right_dock_panel->setMinimumSize(QSize(0, 100));
        right_dock_panel->setMaximumSize(QSize(40, 16777215));
        right_dock_panel->setAcceptDrops(true);
        right_dock_panel->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QStringLiteral("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 38, 479));
        right_dock_panel->setWidget(scrollAreaWidgetContents_2);
        player_panel = new QScrollArea(centralWidget);
        player_panel->setObjectName(QStringLiteral("player_panel"));
        player_panel->setGeometry(QRect(0, 0, 1111, 40));
        sizePolicy.setHeightForWidth(player_panel->sizePolicy().hasHeightForWidth());
        player_panel->setSizePolicy(sizePolicy);
        player_panel->setMinimumSize(QSize(100, 0));
        player_panel->setMaximumSize(QSize(16777215, 40));
        player_panel->setWidgetResizable(true);
        scrollAreaWidgetContents_3 = new QWidget();
        scrollAreaWidgetContents_3->setObjectName(QStringLiteral("scrollAreaWidgetContents_3"));
        scrollAreaWidgetContents_3->setGeometry(QRect(0, 0, 1109, 38));
        pushButton = new QPushButton(scrollAreaWidgetContents_3);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(290, 10, 75, 23));
        lcdNumber = new QLCDNumber(scrollAreaWidgetContents_3);
        lcdNumber->setObjectName(QStringLiteral("lcdNumber"));
        lcdNumber->setGeometry(QRect(10, 10, 111, 23));
        player_panel->setWidget(scrollAreaWidgetContents_3);
        bottom_dock_panel = new QScrollArea(centralWidget);
        bottom_dock_panel->setObjectName(QStringLiteral("bottom_dock_panel"));
        bottom_dock_panel->setGeometry(QRect(0, 560, 1111, 40));
        sizePolicy.setHeightForWidth(bottom_dock_panel->sizePolicy().hasHeightForWidth());
        bottom_dock_panel->setSizePolicy(sizePolicy);
        bottom_dock_panel->setMinimumSize(QSize(100, 0));
        bottom_dock_panel->setMaximumSize(QSize(16777215, 40));
        bottom_dock_panel->setAcceptDrops(true);
        bottom_dock_panel->setWidgetResizable(true);
        scrollAreaWidgetContents_4 = new QWidget();
        scrollAreaWidgetContents_4->setObjectName(QStringLiteral("scrollAreaWidgetContents_4"));
        scrollAreaWidgetContents_4->setGeometry(QRect(0, 0, 1109, 38));
        bottom_dock_panel->setWidget(scrollAreaWidgetContents_4);
        top_dock_panel = new QScrollArea(centralWidget);
        top_dock_panel->setObjectName(QStringLiteral("top_dock_panel"));
        top_dock_panel->setGeometry(QRect(0, 40, 1111, 40));
        sizePolicy.setHeightForWidth(top_dock_panel->sizePolicy().hasHeightForWidth());
        top_dock_panel->setSizePolicy(sizePolicy);
        top_dock_panel->setMinimumSize(QSize(100, 0));
        top_dock_panel->setMaximumSize(QSize(16777215, 40));
        top_dock_panel->setAcceptDrops(true);
        top_dock_panel->setWidgetResizable(true);
        scrollAreaWidgetContents_5 = new QWidget();
        scrollAreaWidgetContents_5->setObjectName(QStringLiteral("scrollAreaWidgetContents_5"));
        scrollAreaWidgetContents_5->setGeometry(QRect(0, 0, 1109, 38));
        top_dock_panel->setWidget(scrollAreaWidgetContents_5);
        tabber = new QTabWidget(centralWidget);
        tabber->setObjectName(QStringLiteral("tabber"));
        tabber->setGeometry(QRect(70, 100, 971, 441));
        tabber->setMinimumSize(QSize(100, 100));
        tabber->setAcceptDrops(true);
        tabber->setTabShape(QTabWidget::Triangular);
        tabber->setUsesScrollButtons(true);
        tabber->setTabsClosable(true);
        tabber->setMovable(true);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        tabber->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        tabber->addTab(tab_2, QString());
        toolButton = new QToolButton(centralWidget);
        toolButton->setObjectName(QStringLiteral("toolButton"));
        toolButton->setGeometry(QRect(53, 100, 16, 441));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(toolButton->sizePolicy().hasHeightForWidth());
        toolButton->setSizePolicy(sizePolicy1);
        toolButton->setMinimumSize(QSize(16, 100));
        toolButton->setMaximumSize(QSize(16, 16777215));
        toolButton_2 = new QToolButton(centralWidget);
        toolButton_2->setObjectName(QStringLiteral("toolButton_2"));
        toolButton_2->setGeometry(QRect(1042, 100, 16, 441));
        sizePolicy1.setHeightForWidth(toolButton_2->sizePolicy().hasHeightForWidth());
        toolButton_2->setSizePolicy(sizePolicy1);
        toolButton_2->setMinimumSize(QSize(16, 100));
        toolButton_2->setMaximumSize(QSize(16, 16777215));
        toolButton_3 = new QToolButton(centralWidget);
        toolButton_3->setObjectName(QStringLiteral("toolButton_3"));
        toolButton_3->setGeometry(QRect(60, 83, 991, 16));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(toolButton_3->sizePolicy().hasHeightForWidth());
        toolButton_3->setSizePolicy(sizePolicy2);
        toolButton_3->setMinimumSize(QSize(100, 16));
        toolButton_3->setMaximumSize(QSize(16777215, 16));
        toolButton_4 = new QToolButton(centralWidget);
        toolButton_4->setObjectName(QStringLiteral("toolButton_4"));
        toolButton_4->setGeometry(QRect(60, 542, 991, 16));
        sizePolicy2.setHeightForWidth(toolButton_4->sizePolicy().hasHeightForWidth());
        toolButton_4->setSizePolicy(sizePolicy2);
        toolButton_4->setMinimumSize(QSize(100, 16));
        toolButton_4->setMaximumSize(QSize(16777215, 16));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1111, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        QWidget::setTabOrder(toolButton_3, right_dock_panel);
        QWidget::setTabOrder(right_dock_panel, player_panel);
        QWidget::setTabOrder(player_panel, pushButton);
        QWidget::setTabOrder(pushButton, bottom_dock_panel);
        QWidget::setTabOrder(bottom_dock_panel, top_dock_panel);
        QWidget::setTabOrder(top_dock_panel, tabber);
        QWidget::setTabOrder(tabber, toolButton);
        QWidget::setTabOrder(toolButton, toolButton_2);
        QWidget::setTabOrder(toolButton_2, left_dock_panel);
        QWidget::setTabOrder(left_dock_panel, toolButton_4);

        retranslateUi(MainWindow);

        tabber->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        pushButton->setText(QApplication::translate("MainWindow", "Play", 0));
        tabber->setTabText(tabber->indexOf(tab), QApplication::translate("MainWindow", "Tab 1", 0));
        tabber->setTabText(tabber->indexOf(tab_2), QApplication::translate("MainWindow", "Tab 2", 0));
        toolButton->setText(QString());
        toolButton_2->setText(QString());
        toolButton_3->setText(QApplication::translate("MainWindow", "...", 0));
        toolButton_4->setText(QApplication::translate("MainWindow", "...", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
