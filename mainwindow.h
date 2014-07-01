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

#include "data_store.h"

#include "dialogs/tabdialog.h"
#include "dialogs/web_dialog.h"
#include "dialogs/settings_dialog.h"
#include "dialogs/relationdialog.h"

#include "misc/hotkey_manager.h"

#include "override/tabber.h"
#include "override/tab.h"

#include "media/library.h"
#include "media/player.h"
#include "media/mediainfo.h"

#include "toolbars.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initialization();

    void registrateTray();

    QDockWidget * createDockWidget();  

public slots:
    void outputActiveItem(ModelItem *, ModelItem *);
    void receiveMessage(QString);
    void showAttCurrTabDialog();

private slots:
    void showActiveElem();

    void showSettingsDialog();

    void showVKRelTabDialog();
    void showSoundcloudRelTabDialog();

    void showVKTabDialog();
    void showSoundcloudTabDialog();

    void showAttTabDialog(Tab * tab = 0);

    void openFolderTriggered();

    void nextItemTriggered();
    void nextItemWithDelTriggered();
    void prevItemTriggered();

//    void dropEvent(QDropEvent * event);


protected:
    void putToCommonTab(QList<QUrl> urls);
    void dragEnterEvent(QDragEnterEvent *);
    void dragMoveEvent(QDragMoveEvent *);
    void dropEvent(QDropEvent *);
    void closeEvent(QCloseEvent *event);
    QMenu* createPopupMenu();

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon m_tray;

    DataStore * settings;
};

#endif // MAINWINDOW_H
