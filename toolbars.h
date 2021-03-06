#ifndef TOOLBARS_H
#define TOOLBARS_H

#include <QToolBar>
#include <QMainWindow>

#include "misc/data_store.h"

#include "web/socials/vk_api.h"
#include "web/socials/soundcloud_api.h"

#include "override/toolbar.h"
#include "override/toolbarbutton.h"
#include "override/slider_style.h"
#include "override/clickable_label.h"
#include "override/slider.h"
#include "override/spectrum.h"

#include "dialogs/toolbardialog.h"
#include "dialogs/toolbarbuttondialog.h"

class ToolBars : public QObject {
    Q_OBJECT
public:
    ~ToolBars() {
        delete underMouseBar;
        if (vkToolButton)
            delete vkToolButton -> menu();
        delete vkToolButton;

        if (soundcloudToolButton)
            delete soundcloudToolButton -> menu();
        delete soundcloudToolButton;

        delete spectrum;
    }

    static ToolBars * instance(QObject * parent);
    static void close() {
        delete self;
    }

    QMenu * improvePopupMenu(QMainWindow * window, QMenu * menu);

    void load(QMainWindow * window, QJsonArray & bars);
    void save(QMainWindow * window, DataStore * settings);
    void createToolbars(QMainWindow * window);
    void recreateToolbars(QMainWindow * window, QList<QString> & required);
    QToolBar * linkNameToToolbars(QString barName);


    QToolBar* createToolBar(QString name);
    QToolBar* createMediaBar();
    QToolBar* createAdditionalMediaBar();
    QToolBar* createPositionMediaBar();
    QToolBar* createTimeMediaBar();
    QToolBar* createVolumeMediaBar();
    QToolBar* createControlToolBar();
    Spectrum* getSpectrum();


    QToolButton * initiateVkButton();
    QToolButton * initiateSoundcloudButton();

    void addPanelButton(QString name, QString path, QToolBar * bar);

    bool isToolbarNameUniq(QMainWindow * window, QString name);

public slots:
    void folderDropped(QString name, QString path);
    void changeToolbarMovable();
    void changeToolbarsMovable();
    void toolbarVisibilityChanged(bool visible);
    void toolbarOrientationChanged(Qt::Orientation);

    void removePanelHighlight();
    void panelHighlight(QAction *action = 0);
    void addPanelTriggered();
    void removePanelTriggered();
    void addPanelButtonTriggered();
    void removePanelButtonTriggered();
private:
    ToolBars(QObject * parent) : QObject(parent) {
        highlighted = 0;
        vkToolButton = 0;
        soundcloudToolButton = 0;
        spectrum = 0;

        activeBar = 0;
        underMouseBar = 0;
        underMouseButton = 0;

        pal.setColor(QPalette::Button, QColor("#E1E0E0")); // background color for not droppable panels
    }

    QToolButton * vkToolButton;
    QToolButton * soundcloudToolButton;

    QWidget * highlighted;

    Spectrum * spectrum;
    QToolBar * underMouseBar, * activeBar;
    ToolbarButton * underMouseButton;

    QPalette pal;
    QPoint lastClickPoint;

    static ToolBars *self;
};

#endif // TOOLBARS_H
