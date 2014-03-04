#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QKeySequence>
#include <QToolBar>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QDesktopServices>
#include <QWidgetAction>

#include "globalshortcut/qxtglobalshortcut.h"
#include "tabber.h"
#include "tab.h"
#include "data_store.h"
#include "player.h"
#include "toolbar.h"
#include "toolbarbutton.h"

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
    QToolBar* createControlToolBar();
    void createToolbars();
    void addPanelButton(QString name, QString path, QToolBar * bar);

private slots:
    void showAttTabDialog();
    void mediaVisibilityChanged(bool visible);
    void slotNoImpl();
    void addPanelTriggered();
    void addPanelButtonTriggered();
    void OpenFolderTriggered();
    void folderDropped(QString name, QString path);


//    void dropEvent(QDropEvent * event);


protected:
    void closeEvent(QCloseEvent *event);
    QMenu* createPopupMenu ();

private:
    Ui::MainWindow *ui;
    Tabber * tabber;
    QTimer autoSaveTimer;
    QSystemTrayIcon m_tray;
    QToolBar * underMouseBar;

    QxtGlobalShortcut * next;
    QxtGlobalShortcut * next_and_delete;
    QxtGlobalShortcut * prev;

    DataStore * settings;
};

#endif // MAINWINDOW_H
