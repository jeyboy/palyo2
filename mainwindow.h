#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QSettings>

#include "misc/data_store.h"

#include "dialogs/tabdialog.h"
#include "dialogs/web_dialog.h"
#include "dialogs/settings_dialog.h"
#include "dialogs/relationdialog.h"

#include "override/tab.h"

#include "media/library.h"
#include "media/player.h"

#include "media/media_player/media_player.h"

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
    void locationCorrection();

    void registrateTray();

    QDockWidget * createDockWidget();  

public slots:
    void outputActiveItem(ModelItem *, ModelItem *);
    void receiveMessage(QString);
    void showAttCurrTabDialog();

private slots:
    void showError(QString);

    void showActiveElem();

    void showSettingsDialog();

    void showVKRelTabDialog();
    void showSoundcloudRelTabDialog();

    void openVKTabDialog();
    void showVKTabDialog();
    void openSoundcloudTabDialog();
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
