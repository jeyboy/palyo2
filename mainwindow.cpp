#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tabdialog.h"
#include "toolbardialog.h"
#include "toolbarbuttondialog.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QDebug>


QMenu * MainWindow::createPopupMenu () {   
    QMenu *menu = QMainWindow::createPopupMenu();

    menu -> insertSeparator(menu->actions().first());

    QAction * addButtonAct = new QAction("Add button", menu);
    QWidget * widget = this -> childAt(this -> mapFromGlobal(QCursor::pos()));
    underMouseBar = ((ToolBar*)widget);
    addButtonAct -> setEnabled(QString(widget -> metaObject() -> className()) == "ToolBar");
    menu -> insertAction(menu->actions().first(), addButtonAct);
    connect(addButtonAct, SIGNAL(triggered(bool)), this, SLOT(addPanelButtonTriggered()));

    QAction * act = new QAction("Add panel", menu);
    connect(act, SIGNAL(triggered(bool)), this, SLOT(addPanelTriggered()));
    menu -> insertAction(menu->actions().first(), act);

    return menu;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    ///////////////////////////////////////////////////////////
    ///settings bla bla bla
    ///////////////////////////////////////////////////////////
    settings = new DataStore("settings.json");
    int left = settings->read("left").toInt();
    int top = settings->read("top").toInt();

    QDesktopWidget *desktop = QApplication::desktop();

    if (left >= desktop->width())
        left = desktop->width() - 50;

    if (top >= desktop->height())
        top = desktop->height() - 50;

    move(left, top);
    resize(settings->read("width").toInt(), settings->read("height").toInt());
    ///////////////////////////////////////////////////////////
    /// toolbars
    ///////////////////////////////////////////////////////////
    ///
    underMouseBar = 0;
    ///
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
            } else if (barName == "Controls") {
                curr_bar = createControlToolBar();
            } else {
                curr_bar = createToolBar(barName);
            }



    //        curr_bar -> setAcceptDrops(true);
            curr_bar -> setHidden(obj.value("hidden").toBool());
            curr_bar -> setOrientation((Qt::Orientation)obj.value("orient").toInt());

            addToolBar((Qt::ToolBarArea)obj.value("area").toInt(), curr_bar);
            if (obj.value("break").toBool())
                insertToolBarBreak(curr_bar);

            curr_bar -> move(obj.value("posx").toInt(), obj.value("posy").toInt());

            if (obj.contains("actions")) {
                QJsonArray actions = obj.value("actions").toArray();

                foreach(QJsonValue act, actions) {
                    actionObj = act.toObject();
                    addPanelButton(actionObj.value("name").toString(), actionObj.value("path").toString(), curr_bar);
                }
            }
        }
    } else {
        createToolbars();
    }
    ///////////////////////////////////////////////////////////

    tabber = new Tabber(ui -> tabber);
    registrateGlobalKeys();

//    connect(&autoSaveTimer, SIGNAL(timeout()), this, SLOT(autoSave()));
//    autoSaveTimer.start(1000*60*5);//Every 5 minutes
//    QApplication::setWindowIcon(QIcon(":/images/tray.png"));

//    CBHash hash;
//    tabber -> addTab(tr("Test"),  hash);
}

void MainWindow::registrateGlobalKeys() {
   next = new QxtGlobalShortcut(QKeySequence("Ctrl+Down"));
   connect(next, SIGNAL(activated()), this, SLOT(slotNoImpl()));

   next_and_delete = new QxtGlobalShortcut(QKeySequence("Ctrl+Delete"));
   connect(next_and_delete, SIGNAL(activated()), this, SLOT(slotNoImpl()));

   prev = new QxtGlobalShortcut(QKeySequence("Ctrl+Up"));
   connect(prev, SIGNAL(activated()), this, SLOT(slotNoImpl()));
}

void MainWindow::registrateTray() {
//    m_tray.setIcon(QIcon(":/images/tray.png"));

//    if(m_tray.isSystemTrayAvailable()) {
//        QMenu *pTrayMenu = new QMenu();
//        pTrayMenu->addAction("Add snippet", this, SLOT(onAddSnippet()),Qt::ControlModifier + Qt::Key_D);
//        pTrayMenu->addAction("Search", this, SLOT(onSearch()),Qt::MetaModifier + Qt::Key_V);
//        pTrayMenu->addSeparator();
//        pTrayMenu->addAction("Save", this, SLOT(save()));
//        pTrayMenu->addSeparator();
//        pTrayMenu->addAction("Exit", this, SLOT(exit()));
//        m_tray.setContextMenu(pTrayMenu);
//        m_tray.show();
//    }
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
    connect(ptb, SIGNAL(visibilityChanged(bool)), this, SLOT(mediaVisibilityChanged(bool)));
    ptb -> setMinimumHeight(30);

    ptb->addAction(QPixmap(QString(":/play")), "Play", this, SLOT(slotNoImpl()));
    ptb->addAction(QPixmap(QString(":/pause")), "Pause", this, SLOT(slotNoImpl()));
    ptb->addAction(QPixmap(QString(":/stop")), "Stop", this, SLOT(slotNoImpl()));
    ptb->addSeparator();

    QSlider * horizontalSlider = new QSlider();
    horizontalSlider -> setOrientation(Qt::Horizontal);
    horizontalSlider -> setMinimumSize(80, 25);

//    setStyleSheet("QSlider::handle {image: url(:/resources/image.png);}");
    horizontalSlider -> setStyleSheet(QString(
                                              "QSlider::groove:horizontal {"
                                                "border: 1px solid #bbb;"
                                                "background: white;"
                                                "height: 18px;"
                                                "border-radius: 4px;"
                                              "}"
                                              "QSlider::sub-page:horizontal {"
                                                "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                                    "stop: 0 #66e, stop: 1 #bbf);"
                                                "background: qlineargradient(x1: 0, y1: 0.2, x2: 1, y2: 1,"
                                                    "stop: 0 #bbf, stop: 1 #55f);"
                                                "border: 1px solid #777;"
                                                "height: 18px;"
                                                "border-radius: 4px;"
                                              "}"
                                              "QSlider::add-page:horizontal {"
                                                "background: #fff;"
                                                "border: 1px solid #777;"
                                                "height: 18px;"
                                                "border-radius: 4px;"
                                              "}"
                                              "QSlider::handle:horizontal {"
                                                "background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
                                                    "stop:0 #eee, stop:1 #ccc);"
                                                "border: 1px solid #777;"
                                                "width: 12px;"
                                                "margin: 0 -1px;"
                                                "border-radius: 4px;"
                                              "}"
                                              "QSlider::handle:horizontal:hover {"
                                                "background: qlineargradient(x1:0, y1:0, x2:1, y2:1,"
                                                    "stop:0 #fff, stop:1 #ddd);"
                                                "border: 1px solid #444;"
                                                "border-radius: 4px;"
                                              "}"
                                              "QSlider::sub-page:horizontal:disabled {"
                                                "background: #bbb;"
                                                "border-color: #999;"
                                              "}"
                                              "QSlider::add-page:horizontal:disabled {"
                                                "background: #eee;"
                                                "border-color: #999;"
                                              "}"
                                              "QSlider::handle:horizontal:disabled {"
                                                "background: #eee;"
                                                "border: 1px solid #aaa;"
                                                "border-radius: 4px;"
                                              "}"
    ));

    Player::setTrackBar(horizontalSlider);

    ptb -> addWidget(horizontalSlider);

    ptb -> addSeparator();
    ptb -> addAction(QPixmap(QString(":/like")), "Liked", this, SLOT(slotNoImpl()));
    ptb -> addAction(QPixmap(QString(":/next")), "Next track", this, SLOT(slotNoImpl()));
    ptb -> adjustSize();

    return ptb;
}

QToolBar* MainWindow::createControlToolBar() {
    QToolBar* ptb = new QToolBar("Controls");
//    ptb -> setMinimumWidth(75);

    ptb->addAction(QPixmap(QString(":/add")), "Add new tab", this, SLOT(showAttTabDialog()));
    ptb->addAction(QPixmap(QString(":/settings")), "Setting for current tab", this, SLOT(slotNoImpl()));
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

//void MainWindow::createMenus() {
//    QMenu * mnCreateTab = new QMenu("Add Tab");
//    QAction *msgAction = new QAction("Add Tab", mnCreateTab);
//    connect(msgAction, SIGNAL(triggered()), this, SLOT(showAttTabDialog));
//    mnCreateTab->addAction(msgAction);

//    ui->menuBar->addMenu(mnCreateTab);
//}


void MainWindow::closeEvent(QCloseEvent *event) {
    QSize ws = size();
    QPoint left_top = this -> mapToGlobal(QPoint(0,0));

    settings-> clear();
    settings->write("left", left_top.x() - 8);
    settings->write("top", left_top.y() - 30);
    settings->write("width", ws.width());
    settings->write("height", ws.height());

    QList<QToolBar *> toolbars = this -> findChildren<QToolBar *>();

    if (toolbars.length() > 0) {
        QJsonArray toolbar_array = QJsonArray();
        QJsonObject curr_tab;
        QList<QAction*> actions;
        ToolbarButton* button;

        foreach(QToolBar * bar, toolbars) {
            curr_tab = QJsonObject();

            curr_tab.insert("area", toolBarArea(bar));
            curr_tab.insert("break", toolBarBreak(bar));
            curr_tab.insert("posx", bar -> pos().x());
            curr_tab.insert("posy", bar -> pos().y());
            curr_tab.insert("hidden", bar -> isHidden());
            curr_tab.insert("orient", bar -> orientation());
            curr_tab.insert("title", bar -> windowTitle());

            if (bar -> windowTitle() != "Media" && bar -> windowTitle() != "Controls") {
                actions = bar -> actions();
                if (actions.length() > 0) {
                    QJsonArray action_array = QJsonArray();
                    QJsonObject curr_act;

                    foreach(QAction * act, actions) {
                        if (QString(act -> metaObject() -> className()) == "QWidgetAction") {
                            curr_act = QJsonObject();
                            button = (ToolbarButton*) bar -> widgetForAction(act);// (((QWidgetAction*)act) -> defaultWidget());

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
    tabber -> save();

    event ->accept();
}

MainWindow::~MainWindow() {
    delete ui;
    delete next;
    delete next_and_delete;
    delete prev;
    delete settings;
}

void MainWindow::addPanelButton(QString name, QString path, QToolBar * bar) {
    ToolbarButton * button = new ToolbarButton(name, path);
    bar -> addWidget(button);
    connect(button, SIGNAL(clicked()), this, SLOT(OpenFolderTriggered()));
}

/////////////////////////////////////////////////////////////////////////////////////
///SLOTS
/////////////////////////////////////////////////////////////////////////////////////

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

void MainWindow::addPanelButtonTriggered() {
    ToolbarButtonDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        addPanelButton(dialog.getName(), dialog.getPath(), underMouseBar);
    }
}

void MainWindow::OpenFolderTriggered() {
    ToolbarButton * button = (ToolbarButton*)QObject::sender();
    QDesktopServices::openUrl(QUrl::fromLocalFile(button -> path));
}

void MainWindow::slotNoImpl() {
    QMessageBox::information(0, "Message", "Not implemented");
}

void MainWindow::mediaVisibilityChanged(bool visible) {
  if (visible == false)
    ((QToolBar*)QObject::sender()) -> setVisible(true);
}

void MainWindow::showAttTabDialog() {
  TabDialog dialog(this);

  if (dialog.exec() == QDialog::Accepted) {
     tabber -> addTab(dialog.getName(), dialog.getSettings());
     qDebug() << "Settings: " << dialog.getSettings();
    }
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
