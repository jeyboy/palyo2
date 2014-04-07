#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tabdialog.h"
#include "toolbardialog.h"
#include "toolbarbuttondialog.h"
#include "mediainfo.h"

#include <QDebug>

QMenu * MainWindow::createPopupMenu () {   
    //TODO: add label with name on panel
    //TODO: add panel highlight on mouse over in menu
    QMenu *menu = QMainWindow::createPopupMenu();

//    menu -> insertSeparator(menu->actions().first());
    menu -> insertSection(menu->actions().first(), "Panels list");

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

    return menu;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    this -> setWindowTitle("Playo");
    pal.setColor(QPalette::Button, QColor("#E1E0E0"));

    QSettings stateSettings("settings.ini", QSettings::IniFormat, this);

    QVariant geometryState = stateSettings.value("geometry");
    QVariant objState = stateSettings.value("windowState");

    settings = new DataStore("settings.json");

    if (geometryState.isValid())
        restoreGeometry(geometryState.toByteArray());

    ///////////////////////////////////////////////////////////
    ///location correction (test needed)
    ///////////////////////////////////////////////////////////

    QDesktopWidget *desktop = QApplication::desktop();
    int left = x(), top = y();


    if (left >= desktop->width())
        left = desktop->width() - 50;

    if (top >= desktop->height())
        top = desktop->height() - 50;

    move(left, top);

    ///////////////////////////////////////////////////////////
    /// toolbars
    ///////////////////////////////////////////////////////////

    underMouseBar = 0;
    underMouseButton = 0;

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
    registrateGlobalKeys();
    registrateTray();

//    connect(&autoSaveTimer, SIGNAL(timeout()), this, SLOT(autoSave()));
//    autoSaveTimer.start(1000*60*5);//Every 5 minutes
    QApplication::setWindowIcon(QIcon(":icon"));


//    MediaInfo m("C:\\Josh Whelchel - Demoninution.mp3");
//    m.initInfo();
}

void MainWindow::registrateGlobalKeys() {
   next = new QxtGlobalShortcut(QKeySequence("Ctrl+Down"));
   connect(next, SIGNAL(activated()), this, SLOT(nextItemTriggered()));

   next_and_delete = new QxtGlobalShortcut(QKeySequence("Ctrl+Delete"));
   connect(next_and_delete, SIGNAL(activated()), this, SLOT(nextItemWithDelTriggered()));

   prev = new QxtGlobalShortcut(QKeySequence("Ctrl+Up"));
   connect(prev, SIGNAL(activated()), this, SLOT(prevItemTriggered()));
}

//TODO: menu finish needed
void MainWindow::registrateTray() {
    m_tray.setIcon(QIcon(":/icon"));

    if(m_tray.isSystemTrayAvailable()) {
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


    Player::instance() -> setPlayButton(ptb->addAction(QPixmap(":/play"), "Play"));
    Player::instance() -> setPauseButton(ptb->addAction(QPixmap(":/pause"), "Pause"));
    Player::instance() -> setStopButton(ptb->addAction(QPixmap(":/stop"), "Stop"));

    ptb -> adjustSize();

    return ptb;
}

QToolBar* MainWindow::createTimeMediaBar() {
    QToolBar* ptb = new QToolBar("Media+Time");
    ptb -> setObjectName("_Media+Time");

    ptb -> setAutoFillBackground(true);
    ptb -> setPalette(pal);
    ptb -> setMinimumHeight(30);

    QLCDNumber * timeLabel = new QLCDNumber();
//    timeLabel -> setDigitCount(8);
    timeLabel -> setSegmentStyle(QLCDNumber::Flat);
    timeLabel -> display("00:00");
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

    QSlider * slider = new QSlider();
    slider -> setStyle(new SliderStyle());
    slider -> setOrientation(Qt::Horizontal);
    slider -> setMinimumSize(30, 30);
//    slider -> setTracking(false); // send change value only on release slider

//    "QSlider::sub-page {"
//      "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
//          "stop: 0 #66e, stop: 1 #bbf);"
//      "background: qlineargradient(x1: 0, y1: 0.2, x2: 1, y2: 1,"
//          "stop: 0 #bbf, stop: 1 #55f);"
//      "border: 1px solid #777;"
//      "border-radius: 4px;"
//    "}"

//    "background-image: url(:/progress);"

//    setStyleSheet("QSlider::handle {image: url(:/resources/image.png);}");
    slider -> setStyleSheet(QString(
                                              "QSlider::groove {"
                                                "border: 1px solid #bbb;"
                                                "background-image: url(:/slideback);"
                                                "background-position: center center;"
                                                "border-radius: 4px;"
                                              "}"

                                              "QSlider::groove:horizontal {"
                                                "height: 18px;"
                                              "}"

                                              "QSlider::groove:vertical {"
                                                "width: 18px;"
                                              "}"

                                              "QSlider::add-page {"
                                                "background-color: rgba(255, 255, 255, 100%);"
                                              "}"

                                              "QSlider::sub-page {"
                                                "background-color: rgba(255, 255, 255, 30%);"
                                                "border: 1px solid #777;"
                                                "border-radius: 4px;"
                                              "}"

                                              "QSlider::sub-page:horizontal {"
                                                "height: 18px;"
                                              "}"

                                              "QSlider::sub-page:vertical {"
                                                "width: 18px;"
                                              "}"

                                              "QSlider::add-page {"
                                                "background: #fff;"
                                                "border: 1px solid #777;"
                                                "border-radius: 4px;"
                                              "}"

                                              "QSlider::add-page:horizontal {"
                                                "height: 18px;"
                                              "}"

                                              "QSlider::add-page:vertical {"
                                                "width: 18px;"
                                              "}"

                                              "QSlider::handle {"
                                                "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #eee, stop:1 #ccc);"
                                                "border: 1px solid #777;"
                                                "margin: 0 -1px;"
                                                "border-radius: 4px;"
                                              "}"

                                              "QSlider::handle:horizontal {"
                                                "width: 12px;"
                                              "}"

                                              "QSlider::handle:vertical {"
                                                "height: 12px;"
                                              "}"

                                              "QSlider::handle:hover {"
                                                "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #fff, stop:1 #ddd);"
                                                "border: 1px solid #444;"
                                                "border-radius: 4px;"
                                              "}"

                                              "QSlider::sub-page:disabled {"
                                                "background: #bbb;"
                                                "border-color: #999;"
                                              "}"

                                              "QSlider::add-page:disabled {"
                                                "background: #eee;"
                                                "border-color: #999;"
                                              "}"

                                              "QSlider::handle:disabled {"
                                                "background: #eee;"
                                                "border: 1px solid #aaa;"
                                                "border-radius: 4px;"
                                              "}"
    ));


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

    ptb->addAction(QPixmap(QString(":/add")), "Add new tab", this, SLOT(showAttTabDialog()));
    ptb->addAction(QPixmap(QString(":/settings")), "Setting for current tab", this, SLOT(showAttCurrTabDialog()));
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

void MainWindow::closeEvent(QCloseEvent *event) {
    settings-> clear();

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

    settings -> save();

    QSettings stateSettings("settings.ini", QSettings::IniFormat, this);
    stateSettings.setValue("geometry", saveGeometry());
    stateSettings.setValue("windowState", saveState());
    stateSettings.sync();

    tabber -> save();

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
    ///////////////////////////////////////////////

    delete next;
    delete next_and_delete;
    delete prev;

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

void MainWindow::addPanelTriggered() {
//    ((QAction*)QObject::sender()) -> userData(0);
//    qDebug() << QObject::sender();

    ToolbarDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        addToolBar(Qt::BottomToolBarArea, createToolBar(dialog.getName()));
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

void MainWindow::slotNoImpl() {
//    QMessageBox::information(0, "Message", "Not implemented");
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
        orientable -> setInvertedAppearance(orientation == Qt::Vertical);
    }
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
}








//void MainWindow::initMedia() {

////        sourceFile.setFileName("/tmp/test.raw");
////        sourceFile.open(QIODevice::ReadOnly);

////        QAudioFormat format;
////        // Set up the format, eg.
////        format.setSampleRate(8000);
////        format.setChannelCount(1);
////        format.setSampleSize(8);
////        format.setCodec("audio/pcm");
////        format.setByteOrder(QAudioFormat::LittleEndian);
////        format.setSampleType(QAudioFormat::UnSignedInt);

////        QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
////        if (!info.isFormatSupported(format)) {
////            qWarning() << "Raw audio format not supported by backend, cannot play audio.";
////            return;
////        }

////    audio = new QAudioOutput(format, this);

//    audio = new QMediaPlayer();
//    QObject::connect(audio, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChanged(QAudio::State)));
//}

//void MainWindow::handleStateChanged(QAudio::State newState)
//{
//    switch (newState) {
//        case QAudio::IdleState:
//            // Finished playing (no more data)
//            audio -> stop();
////            sourceFile.close();
//            delete audio;
//            break;

//        case QAudio::StoppedState:
//            // Stopped for other reasons
//            if (audio -> error() != QAudio::NoError) {
//                // Error handling
//            }
//            break;

//        default:
//            // ... other cases as appropriate
//            break;
//    }
//}

//void MainWindow::on_pushButton_clicked(){
////player->setMedia(QUrl::fromLocalFile("/Users/me/Music/coolsong.mp3"));


////    playlist = new QMediaPlaylist;
////    playlist->addMedia(QUrl("http://example.com/movie1.mp4"));
////    playlist->addMedia(QUrl("http://example.com/movie2.mp4"));
////    playlist->addMedia(QUrl("http://example.com/movie3.mp4"));
////    playlist->setCurrentIndex(1);

////    player = new QMediaPlayer;
////    player->setPlaylist(playlist);

////    videoWidget = new QVideoWidget;
////    player->setVideoOutput(videoWidget);
////    videoWidget->show();

////    player->play();



////    QString path = QFileDialog::getOpenFileName(this, tr("Choose file"));
////    player = new QMediaPlayer();
////////    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
////    player -> setMedia(QUrl::fromLocalFile(path));
////    player -> play();
//}

//void MainWindow::on_tabWidget_tabCloseRequested(int index) {
////   QMessageBox::information(this, tr("Удаление"), QString("Удаление таба %1").arg(index));
////   QString("Целое %1 и дробное %2").arg(a).arg(b)

////    QWidget *pWin = QApplication::activeWindow;
//////    QObjectList *pList = pWin->children();
////    QTabWidget* tabber = pWin->findChild<QTabWidget*>("tabber");
////    tabber -> addTab(tabber, QString('Hudo'));
//}
