#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

QMenu * MainWindow::createPopupMenu () {   
    //TODO: add label with name on panel
    QMenu *menu = QMainWindow::createPopupMenu();
    connect(menu, SIGNAL(hovered(QAction *)), this, SLOT(panelHighlight(QAction *)));
    connect(menu, SIGNAL(aboutToHide()), this, SLOT(removePanelHighlight()));

//    menu -> insertSeparator(menu->actions().first());
    menu -> insertSection(menu -> actions().first(), "Panels list");

    lastClickPoint = QCursor::pos();
    QWidget * widget = this -> childAt(this -> mapFromGlobal(lastClickPoint));
    QString widgetClassName = QString(widget -> metaObject() -> className());


    if (widgetClassName == "ToolbarButton") {
        underMouseButton = ((ToolbarButton*)widget);
        underMouseBar = ((ToolBar*)underMouseButton -> parentWidget());
    } else {
        underMouseBar = ((ToolBar*)widget);
    }

    QAction * removeButtonAct = new QAction(QIcon(":drop_remove"), "Remove drop point", menu);
    removeButtonAct -> setEnabled(widgetClassName == "ToolbarButton");
    menu -> insertAction(menu->actions().first(), removeButtonAct);
    connect(removeButtonAct, SIGNAL(triggered(bool)), this, SLOT(removePanelButtonTriggered()));


    QAction * addButtonAct = new QAction(QIcon(":drop_add"), "Add drop point", menu);
    addButtonAct -> setEnabled(widgetClassName == "ToolBar");
    menu -> insertAction(menu->actions().first(), addButtonAct);
    connect(addButtonAct, SIGNAL(triggered(bool)), this, SLOT(addPanelButtonTriggered()));

    menu -> insertSection(menu->actions().first(), QIcon(":drops"),  "Drop points");

    QAction * removePanelAct = new QAction(QIcon(":panel_remove"), "Remove panel", menu);
    removePanelAct -> setEnabled(widgetClassName == "ToolBar");
    connect(removePanelAct, SIGNAL(triggered(bool)), this, SLOT(removePanelTriggered()));
    menu -> insertAction(menu->actions().first(), removePanelAct);

    QAction * addPanelAct = new QAction(QIcon(":panel_add"), "Add panel", menu);
    connect(addPanelAct, SIGNAL(triggered(bool)), this, SLOT(addPanelTriggered()));
    menu -> insertAction(menu->actions().first(), addPanelAct);

    menu -> insertSection(menu->actions().first(), QIcon(":panels"), "Panel");

    //    activeBar

    ////////////////////////// for bar movable fixing ////////////////////////////////
    if (widgetClassName == "QToolBar" || widgetClassName == "ToolBar") {
        activeBar = ((QToolBar*)widget);
    } else {
        activeBar = ((QToolBar*)widget -> parentWidget());
    }

    QAction * fixToolbarAct;

    if (activeBar -> isMovable()) {
        fixToolbarAct = new QAction(QIcon(":locked"), "Static bar", menu);
    } else {
        fixToolbarAct = new QAction(QIcon(":unlocked"), "Movable bar", menu);
    }

    menu -> insertAction(menu -> actions().first(), fixToolbarAct);
    connect(fixToolbarAct, SIGNAL(triggered(bool)), this, SLOT(changeToolbarMovable()));

    //////////////////////////////////////////////////////////////////////////////////

    return menu;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {
    ui -> setupUi(this);

    setWindowTitle("Playo");
    setAcceptDrops(true);
    pal.setColor(QPalette::Button, QColor("#E1E0E0"));
    highlighted = 0;

    QSettings stateSettings("settings.ini", QSettings::IniFormat, this);

    QVariant geometryState = stateSettings.value("geometry");
    QVariant objState = stateSettings.value("windowState");

    settings = new DataStore("settings.json");

    qDebug() << "Settings ip " << settings -> read("ip").toString();
    IpChecker::instance(settings -> read("ip").toString());

    if (geometryState.isValid())
        restoreGeometry(geometryState.toByteArray());

    ///////////////////////////////////////////////////////////
    ///location correction (test needed)
    ///////////////////////////////////////////////////////////

    QDesktopWidget *desktop = QApplication::desktop();
    int left = x(), top = y();


    if (left >= desktop -> width())
        left = desktop -> width() - 50;

    if (top >= desktop -> height())
        top = desktop -> height() - 50;

    move(left, top);

    ///////////////////////////////////////////////////////////
    /// toolbars
    ///////////////////////////////////////////////////////////

    activeBar = 0;
    underMouseBar = 0;
    underMouseButton = 0;

    Settings::instance() -> fromJson(settings -> read("settings").toObject());
    SettingsDialog::registerHotkeys(this);

    QJsonArray bars = settings -> read("bars").toArray();

    if (bars.count() > 0) {
        QJsonObject obj, actionObj;
        QString barName;
        QToolBar * curr_bar;

        foreach(QJsonValue bar, bars) {
            obj = bar.toObject();
            barName = obj.value("title").toString();

            if (barName == "Media") {
                curr_bar = createMediaBar();
            } else if (barName == "Media+") {
                curr_bar = createAdditionalMediaBar();
            } else if (barName == "Media+Position") {
                curr_bar = createPositionMediaBar();
            } else if (barName == "Media+Time") {
                curr_bar = createTimeMediaBar();
            } else if (barName == "Media+Volume") {
                curr_bar = createVolumeMediaBar();
            } else if (barName == "Controls") {
                curr_bar = createControlToolBar();
            } else {
                curr_bar = createToolBar(barName);
            }

            addToolBar((Qt::ToolBarArea)obj.value("area").toInt(), curr_bar);

            if (obj.contains("actions")) {
                QJsonArray actions = obj.value("actions").toArray();

                foreach(QJsonValue act, actions) {
                    actionObj = act.toObject();
                    addPanelButton(actionObj.value("name").toString(), actionObj.value("path").toString(), curr_bar);
                }
            }
        }

        if (objState.isValid())
            restoreState(objState.toByteArray());
    } else {
        createToolbars();
    }
    ///////////////////////////////////////////////////////////

    tabber = new Tabber(ui -> tabber);
    registrateTray();

    QApplication::setWindowIcon(QIcon(":icon"));

    connect(Player::instance(), SIGNAL(itemChanged(ModelItem *, ModelItem *)), this, SLOT(outputActiveItem(ModelItem *, ModelItem *)));

//    qDebug() << Library::instance() -> prepareName("5ugar, Eva Kade  (2012) http://vk.com/clubmusicthebestin - All Around feat Evil T (Andrea Bertolini Remix)");
    showActiveElem();
}

//TODO: menu finish needed
void MainWindow::registrateTray() {
    m_tray.setIcon(QIcon(":/icon"));

    if(m_tray.isSystemTrayAvailable()) {
//        m_tray.showMessage();
        QMenu *pTrayMenu = new QMenu();
//        pTrayMenu->addAction("Add snippet", this, SLOT(onAddSnippet()),Qt::ControlModifier + Qt::Key_D);
//        pTrayMenu->addAction("Search", this, SLOT(onSearch()),Qt::MetaModifier + Qt::Key_V);
//        pTrayMenu->addSeparator();
//        pTrayMenu->addAction("Save", this, SLOT(save()));
//        pTrayMenu->addSeparator();
//        pTrayMenu->addAction("Exit", this, SLOT(exit()));
        m_tray.setContextMenu(pTrayMenu);
        m_tray.show();
    }
}

void MainWindow::createToolbars() {

//    addDockWidget(Qt::LeftDockWidgetArea, createDockWidget());

//  QToolBar * barTest = addToolBar(tr("aaaa"));
//  .....// some actions in the first row
//  this->addToolBarBreak();
//  QToolBar * barTest2 = addToolBar(tr("bbbb"));
//  .....// some other actions in the second row

//  addDockWidget

  addToolBar(Qt::TopToolBarArea, createMediaBar());
  addToolBar(Qt::TopToolBarArea, createTimeMediaBar());
  addToolBar(Qt::TopToolBarArea, createPositionMediaBar());
  addToolBarBreak();
  addToolBar(Qt::TopToolBarArea, createAdditionalMediaBar());
  addToolBar(Qt::TopToolBarArea, createVolumeMediaBar());
  addToolBar(Qt::TopToolBarArea, createControlToolBar());
  addToolBar(Qt::BottomToolBarArea, createToolBar("Folder linker 1"));
}

QDockWidget * MainWindow::createDockWidget() {
//    QDockWidget *dock = new QDockWidget(tr("Customers"), this);
//    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
//    customerList = new QListWidget(dock);
//    customerList->addItems(QStringList()
//            << "John Doe, Harmony Enterprises, 12 Lakeside, Ambleton"
//            << "Jane Doe, Memorabilia, 23 Watersedge, Beaton"
//            << "Tammy Shea, Tiblanka, 38 Sea Views, Carlton"
//            << "Tim Sheen, Caraba Gifts, 48 Ocean Way, Deal"
//            << "Sol Harvey, Chicos Coffee, 53 New Springs, Eccleston"
//            << "Sally Hobart, Tiroli Tea, 67 Long River, Fedula");
//    dock->setWidget(customerList);
//    addDockWidget(Qt::RightDockWidgetArea, dock);
//    viewMenu->addAction(dock->toggleViewAction());
}

QToolBar* MainWindow::createMediaBar() {
    QToolBar* ptb = new QToolBar("Media");
    ptb -> setObjectName("_Media");

    ptb -> setAutoFillBackground(true);
    ptb -> setPalette(pal);

    connect(ptb, SIGNAL(visibilityChanged(bool)), this, SLOT(mediaVisibilityChanged(bool)));
//    connect(ptb, SIGNAL(orientationChanged(Qt::Orientation)), this, SLOT(mediaOrientationChanged(Qt::Orientation)));
    ptb -> setMinimumHeight(30);


    Player::instance() -> setPlayButton(ptb -> addAction(QPixmap(":/play"), "Play"));
    Player::instance() -> setPauseButton(ptb -> addAction(QPixmap(":/pause"), "Pause"));
    Player::instance() -> setStopButton(ptb -> addAction(QPixmap(":/stop"), "Stop"));

    ptb -> adjustSize();

    return ptb;
}

QToolBar* MainWindow::createVolumeMediaBar() {
    QToolBar* ptb = new QToolBar("Media+Volume");
    ptb -> setObjectName("_Media+Volume");

    ptb -> setAutoFillBackground(true);
    ptb -> setPalette(pal);
    ptb -> setMinimumHeight(30);

    connect(ptb, SIGNAL(orientationChanged(Qt::Orientation)), this, SLOT(mediaOrientationChanged(Qt::Orientation)));

    QAction * act = ptb -> addAction(QPixmap(":/mute"), "Mute");

    Player::instance() -> setMuteButton(act);

    Slider * slider = new Slider();
    slider -> setStyle(new SliderStyle());
    slider -> setTickInterval(2000);
    slider -> setOrientation(Qt::Horizontal);
    slider -> setMinimumSize(30, 30);

    Player::instance() -> setVolumeTrackBar(slider);
    ptb -> addWidget(slider);
    ptb -> adjustSize();

    return ptb;
}

QToolBar* MainWindow::createTimeMediaBar() {
    QToolBar* ptb = new QToolBar("Media+Time");
    ptb -> setObjectName("_Media+Time");

    ptb -> setAutoFillBackground(true);
    ptb -> setPalette(pal);
    ptb -> setMinimumHeight(30);

    ClickableLabel * timeLabel = new ClickableLabel("00:00");
//    connect(timeLabel, SIGNAL(mousePressEvent(QMouseEvent *ev)), Player::instance(), SLOT(invertTimeCountdown()));
    timeLabel -> setStyleSheet("QLabel { font-weight: bold; font-size: 12px; }");
    ptb -> addWidget(timeLabel);
    Player::instance() -> setTimePanel(timeLabel);
    ptb -> adjustSize();

    return ptb;
}

QToolBar* MainWindow::createPositionMediaBar() {
    QToolBar* ptb = new QToolBar("Media+Position");
    ptb -> setObjectName("_Media+Position");
    ptb -> setAutoFillBackground(true);
    ptb -> setPalette(pal);
    connect(ptb, SIGNAL(orientationChanged(Qt::Orientation)), this, SLOT(mediaOrientationChanged(Qt::Orientation)));
    ptb -> setMinimumHeight(30);

    Slider * slider = new Slider();
    slider -> setStyle(new SliderStyle());
    slider -> setTickInterval(60000);
    slider -> setOrientation(Qt::Horizontal);
    slider -> setMinimumSize(30, 30);
//    slider -> setTracking(false); // send change value only on release slider

    Player::instance() -> setTrackBar(slider);

    ptb -> addWidget(slider);
    ptb -> adjustSize();

    return ptb;
}

QToolBar* MainWindow::createAdditionalMediaBar() {
    QToolBar* ptb = new QToolBar("Media+");
    ptb -> setObjectName("_Media+");
    ptb -> setAutoFillBackground(true);
    ptb -> setPalette(pal);
//    connect(ptb, SIGNAL(visibilityChanged(bool)), this, SLOT(mediaVisibilityChanged(bool)));
//    connect(ptb, SIGNAL(orientationChanged(Qt::Orientation)), this, SLOT(mediaOrientationChanged(Qt::Orientation)));
    ptb -> setMinimumHeight(30);

    ptb -> addAction(QPixmap(":/prev"), "Prev track", this, SLOT(prevItemTriggered()));
    Player::instance() -> setLikeButton(ptb -> addAction(QPixmap(":/like"), "Liked"));
    ptb -> addAction(QPixmap(":/next"), "Next track", this, SLOT(nextItemTriggered()));
    ptb -> adjustSize();

    return ptb;
}

QToolBar* MainWindow::createControlToolBar() {
    QToolBar* ptb = new QToolBar("Controls");
    ptb -> setObjectName("_Controls");
    ptb -> setAutoFillBackground(true);
    ptb -> setPalette(pal);
//    ptb -> setMinimumWidth(75);

    ptb -> addAction(QPixmap(QString(":/add")), "Add new local tab", this, SLOT(showAttTabDialog()));
    ptb -> addAction(QPixmap(QString(":/add_vk")), "Add VK(vk.com) tab", this, SLOT(showVKTabDialog()));
    ptb -> addSeparator();
    ptb -> addAction(QPixmap(QString(":/settings")), "Common setting", this, SLOT(showSettingsDialog()));
    ptb -> adjustSize();

    return ptb;
}

QToolBar* MainWindow::createToolBar(QString name) {
    ToolBar* ptb = new ToolBar(name, this);
    ptb -> setToolButtonStyle(Qt::ToolButtonTextOnly);
    connect(ptb, SIGNAL(folderDropped(QString, QString)), this, SLOT(folderDropped(QString, QString)));

//    ptb->addAction(QPixmap(QString(":/like")), "1", this, SLOT(slotNoImpl()));
//    ptb->addAction(QPixmap(QString(":/next")), "2", this, SLOT(slotNoImpl()));
//    ptb->addSeparator();
//    ptb->addAction(QPixmap(QString(":/like")), "3", this, SLOT(slotNoImpl()));
//    ptb->addAction(QPixmap(QString(":/next")), "4", this, SLOT(slotNoImpl()));

//    toolbar->setMovable(false);

//    QToolButton * button = new QToolButton(this);
//    toolbar->addWidget(button);

//    //way 1: It display a picture
//    toolbar->setStyleSheet("background-image: url(:/images/toolbarBg)");

//    ptb -> adjustSize();
//    connect(ptb, SIGNAL(eventTriggered(QEvent *)), this, SLOT(ToolbarEvent(QEvent *)));
    return ptb;
}

void MainWindow::dragEnterEvent(QDragEnterEvent * event) {
    if (event -> mimeData() -> hasUrls()) {
        event -> accept();
    } else event -> ignore();
}
void MainWindow::dragMoveEvent(QDragMoveEvent * event) {
    if (event -> mimeData() -> hasUrls()) {
        event -> accept();
    } else event -> ignore();
}

void MainWindow::dropEvent(QDropEvent * event) {
//    if (event -> source() != this && event -> mimeData() -> hasUrls()) {
    if (event -> mimeData() -> hasUrls()) {
        putToCommonTab(event -> mimeData() -> urls());
    } else event -> ignore();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    settings -> clear();

    if (IpChecker::instance() -> isInitialized())
        settings -> write("ip", IpChecker::instance() -> currentIp());

    QList<QToolBar *> toolbars = this -> findChildren<QToolBar *>();
    qDebug() << toolbars.length();

    if (toolbars.length() > 0) {
        QJsonArray toolbar_array = QJsonArray();
        QJsonObject curr_tab;
        QList<QAction*> actions;
        ToolbarButton* button;

        foreach(QToolBar * bar, toolbars) {
            curr_tab = QJsonObject();

            curr_tab.insert("area", toolBarArea(bar));
            curr_tab.insert("title", bar -> windowTitle());

            if (bar -> windowTitle() != "Media"
                    && bar -> windowTitle() != "Media+"
                    && bar -> windowTitle() != "Media+Position"
                    && bar -> windowTitle() != "Media+Time"
                    && bar -> windowTitle() != "Media+Volume"
                    && bar -> windowTitle() != "Controls"
               ) {
                actions = bar -> actions();
                if (actions.length() > 0) {
                    QJsonArray action_array = QJsonArray();
                    QJsonObject curr_act;

                    foreach(QAction * act, actions) {
                        if (QString(act -> metaObject() -> className()) == "QWidgetAction") {
                            curr_act = QJsonObject();
                            button = (ToolbarButton*) bar -> widgetForAction(act);

                            curr_act.insert("path", button -> path);
                            curr_act.insert("name", button -> text());
                        }
                        action_array.append(curr_act);
                    }

                    if (action_array.count() > 0)
                        curr_tab.insert("actions", action_array);
                }
            }

            toolbar_array.append(curr_tab);

    //        bar -> toolButtonStyle();
        }

        settings -> write("bars", toolbar_array);
    }

    settings -> write("settings", Settings::instance() -> toJson());

    settings -> save();

    QSettings stateSettings("settings.ini", QSettings::IniFormat, this);
    stateSettings.setValue("geometry", saveGeometry());
    stateSettings.setValue("windowState", saveState());
    stateSettings.sync();

    tabber -> save();

    m_tray.hide();
    event ->accept();
}

MainWindow::~MainWindow() {
    delete ui;

    ///////////////////////////////////////////////
    /// close singletons
    ///////////////////////////////////////////////
        IconProvider::close();
        Library::close();
        Player::close();

        Settings::close();
        HotkeyManager::close();

        VkApi::close();
    ///////////////////////////////////////////////

    delete settings;
    delete tabber;
    delete underMouseBar;
}

void MainWindow::addPanelButton(QString name, QString path, QToolBar * bar) {
    ToolbarButton * button = new ToolbarButton(name, path);
    bar -> addWidget(button);
    connect(button, SIGNAL(clicked()), this, SLOT(openFolderTriggered()));
}

bool MainWindow::isToolbarNameUniq(QString name) {
    QList<QToolBar *> toolbars = this -> findChildren<QToolBar *>();

    foreach(QToolBar * bar, toolbars) {
        if (bar -> windowTitle() == name)
            return false;
    }

    return true;
}

/////////////////////////////////////////////////////////////////////////////////////
///SLOTS
/////////////////////////////////////////////////////////////////////////////////////

void MainWindow::nextItemTriggered() {
    if (tabber -> currentTab())
        tabber -> currentTab() -> getList() -> proceedNext();
}
void MainWindow::nextItemWithDelTriggered() {
    if (tabber -> currentTab())
        tabber -> currentTab() -> getList() -> deleteCurrentProceedNext();
}
void MainWindow::prevItemTriggered() {
    if (tabber -> currentTab())
        tabber -> currentTab() -> getList() -> proceedPrev();
}

void MainWindow::folderDropped(QString name, QString path) {
    addPanelButton(name, path, (QToolBar*)QObject::sender());
}

void MainWindow::panelHighlight(QAction *action) {
    QString widgetClassName = QString(action -> parentWidget() -> metaObject() -> className());

    if (highlighted != 0)
        emit removePanelHighlight();

    if (widgetClassName == "ToolBar" || widgetClassName == "QToolBar") {
        highlighted = action -> parentWidget();
        highlighted -> setStyleSheet(
                    "QToolBar {"
                      "border: 2px solid red;"
                    "}"
                   );
    }
}

void MainWindow::removePanelHighlight() {
    if (highlighted != 0) {
        highlighted -> setStyleSheet("");
        highlighted = 0;
    }
}

void MainWindow::addPanelTriggered() {
    ToolbarDialog dialog(this);

    while (dialog.exec() == QDialog::Accepted) {
        if (isToolbarNameUniq(dialog.getName())) {
            addToolBar(Qt::BottomToolBarArea, createToolBar(dialog.getName()));
            return;
        }
    }
}

void MainWindow::removePanelTriggered() {
    removeToolBar(underMouseBar);
    delete underMouseBar;
    underMouseBar = 0;
}

void MainWindow::addPanelButtonTriggered() {
    ToolbarButtonDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        addPanelButton(dialog.getName(), dialog.getPath(), underMouseBar);
    }
}

void MainWindow::removePanelButtonTriggered() {
    QToolBar * bar = (QToolBar *)underMouseButton -> parentWidget();
    bar -> removeAction(bar -> actionAt(bar -> mapFromGlobal(lastClickPoint)));
}

void MainWindow::openFolderTriggered() {
    ToolbarButton * button = (ToolbarButton*)QObject::sender();
    QDesktopServices::openUrl(QUrl::fromLocalFile(button -> path));
}

void MainWindow::mediaVisibilityChanged(bool visible) {
  if (visible == false)
    ((QToolBar*)QObject::sender()) -> setVisible(true);
}

void MainWindow::mediaOrientationChanged(Qt::Orientation orientation) {
    QToolBar * bar = (QToolBar*)QObject::sender();

    QList<QSlider *> orientables = bar -> findChildren<QSlider *>();

    foreach(QSlider * orientable, orientables) {
        orientable -> setOrientation(orientation);
//        orientable -> setInvertedAppearance(orientation == Qt::Vertical);
    }
}

void MainWindow::showActiveElem() {
    Tab * tab = tabber -> toActiveTab();
    if (tab)
        tab -> getList() -> scrollToActive();
}

void MainWindow::showSettingsDialog() {
    SettingsDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        if (dialog.isIconSizeChanged())
            tabber -> updateIconSize(dialog.isBigIcon());
    }
}

void MainWindow::showVKRelTabDialog() {
    VkRelationDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        tabber -> addTab("VK (" + dialog.getName() + ")", TabDialog::VKSettings(dialog.getId()));
    } else {
        QMessageBox::information(this, "VK", VkApi::instance() -> getError());
    }
}

void MainWindow::showVKTabDialog() {
    WebDialog dialog(this, VkApi::instance(), "VK auth");
    if (dialog.exec() == QDialog::Accepted) {
        tabber -> addTab("VK", TabDialog::VKSettings());
    } else {
        QMessageBox::information(this, "VK", VkApi::instance() -> getError());
    }
}

void MainWindow::outputActiveItem(ModelItem *, ModelItem * to) {
    if (to && !this -> isActiveWindow())
        m_tray.showMessage("(" + QString::number(tabber -> currentTab() -> getList() -> itemsCount()) + ") Now played:", to -> data(TITLEID).toString(), QSystemTrayIcon::Information, 20000);
}

void MainWindow::putToCommonTab(QList<QUrl> urls) {
    tabber -> commonTab() -> getList() -> dropProcession(urls);
    tabber -> commonTab() -> getList() -> getModel() -> refresh();

    if (!Player::instance() -> isPlayed()) {
        tabber -> commonTab() -> getList() -> proceedNext();
    }
}

void MainWindow::receiveMessage(QString message) {
    QStringList list = message.split('|', QString::SkipEmptyParts);
    QList<QUrl> urls;

    foreach(QString path, list)
        urls.append(QUrl::fromLocalFile(path));

    putToCommonTab(urls);
}

void MainWindow::showAttTabDialog(Tab * tab) {
  TabDialog dialog(this);
  if(tab) {
      qDebug() << tab -> getName();
      dialog.setSettings(tab -> getList() -> getSettings());
      dialog.setName(tab -> getName());

      while(true) {
          if (dialog.exec() == QDialog::Accepted) {
              if (isToolbarNameUniq(dialog.getName())) {
                  tab -> setName(dialog.getName());
                  tab -> getList() -> setSettings(dialog.getSettings());
                  return;
              }
          } else return;
      }
  } else {
      while(true) {
          if (dialog.exec() == QDialog::Accepted) {
              if (isToolbarNameUniq(dialog.getName())) {
                  tabber -> addTab(dialog.getName(), dialog.getSettings());
                  return;
              }
          } else return;
      }
  }
}

void MainWindow::showAttCurrTabDialog() {
    emit showAttTabDialog(tabber -> currentTab());

//    if (tabber -> currentTab() -> isEditable())
//        emit showAttTabDialog(tabber -> currentTab());
//    else
//        QMessageBox::warning(this, "Settings", "This tab type did not have any settings...");
}

void MainWindow::changeToolbarMovable() {
    activeBar -> setMovable(!activeBar -> isMovable());
    activeBar -> repaint();
}
