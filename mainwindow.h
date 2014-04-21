#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QKeySequence>
#include <QToolBar>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QWidgetAction>
#include <QSettings>

#include "globalshortcut/qxtglobalshortcut.h"
#include "tabber.h"
#include "tab.h"
#include "data_store.h"
#include "player.h"
#include "toolbar.h"
#include "toolbarbutton.h"
#include "library.h"
#include "slider_style.h"
#include "clickable_label.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void registrateGlobalKeys();
    void registrateTray();

    QDockWidget * createDockWidget();
    QToolBar* createToolBar(QString name);
    QToolBar* createMediaBar();
    QToolBar* createAdditionalMediaBar();
    QToolBar* createPositionMediaBar();
    QToolBar* createTimeMediaBar();
    QToolBar* createControlToolBar();
    void createToolbars();
    void addPanelButton(QString name, QString path, QToolBar * bar);

protected:
    bool isToolbarNameUniq(QString name);

private slots:
    void showAttTabDialog(Tab * tab = 0);
    void showAttCurrTabDialog();

    void mediaVisibilityChanged(bool visible);
    void mediaOrientationChanged(Qt::Orientation);

    void removePanelHighlight();
    void panelHighlight(QAction *action = 0);
    void addPanelTriggered();
    void removePanelTriggered();
    void addPanelButtonTriggered();
    void removePanelButtonTriggered();
    void openFolderTriggered();
    void folderDropped(QString name, QString path);

    void nextItemTriggered();
    void nextItemWithDelTriggered();
    void prevItemTriggered();


//    void dropEvent(QDropEvent * event);


protected:
    void closeEvent(QCloseEvent *event);
    QMenu* createPopupMenu ();

private:
    Ui::MainWindow *ui;
    Tabber * tabber;
    QSystemTrayIcon m_tray;

    QWidget * highlighted;

    QToolBar * underMouseBar;
    ToolbarButton * underMouseButton;
    QPoint lastClickPoint;

    QxtGlobalShortcut * next;
    QxtGlobalShortcut * next_and_delete;
    QxtGlobalShortcut * prev;
    QxtGlobalShortcut * play;
    QxtGlobalShortcut * stop;

    DataStore * settings;

    QPalette pal;
};

#endif // MAINWINDOW_H
