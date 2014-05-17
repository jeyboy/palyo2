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
#include "dialogs/toolbardialog.h"
#include "dialogs/toolbarbuttondialog.h"
#include "dialogs/web_dialog.h"
#include "dialogs/settings_dialog.h"

#include "misc/hotkey_manager.h"

#include "override/tabber.h"
#include "override/tab.h"
#include "override/toolbar.h"
#include "override/toolbarbutton.h"
#include "override/slider_style.h"
#include "override/clickable_label.h"

#include "media/library.h"
#include "media/player.h"
#include "media/mediainfo.h"

#include "web/socials/vk_api.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

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
    void registerFileType(const QString& documentId, const QString& fileTypeName, const QString& fileExtension, qint32 appIconIndex);

public slots:
    void outputActiveItem(ModelItem *, ModelItem *);
    void receiveMessage(QString);
    void showAttCurrTabDialog();

private slots:
    void showSettingsDialog();

    void showVKTabDialog();

    void showAttTabDialog(Tab * tab = 0);

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
    void putToCommonTab(QList<QUrl> urls);
    void dragEnterEvent(QDragEnterEvent *);
    void dragMoveEvent(QDragMoveEvent *);
    void dropEvent(QDropEvent *);
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
