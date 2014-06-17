#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {
    ui -> setupUi(this);

    QApplication::setWindowIcon(QIcon(":icon"));
    setWindowTitle("Playo");
    setAcceptDrops(true);

    QSettings stateSettings("settings.ini", QSettings::IniFormat, this);
    settings = new DataStore("settings.json");

    ///////////////////////////////////////////////////////////
    ///services loading
    ///////////////////////////////////////////////////////////
    IpChecker::instance(settings -> read("ip").toString());

    VkApi::instance(settings -> read("vk").toObject());
    SoundcloudApi::instance(settings -> read("soundcloud").toObject());

    Settings::instance() -> fromJson(settings -> read("settings").toObject());
    SettingsDialog::registerHotkeys(this);


    QVariant geometryState = stateSettings.value("geometry");
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
    QJsonArray bars = settings -> read("bars").toArray();
    ToolBars::instance(this) -> load(this, bars);

    QVariant objState = stateSettings.value("windowState");
    if (objState.isValid())
        restoreState(objState.toByteArray());
    ///////////////////////////////////////////////////////////

    tabber = new Tabber(ui -> tabber);

    connect(Player::instance(), SIGNAL(itemChanged(ModelItem *, ModelItem *)), this, SLOT(outputActiveItem(ModelItem *, ModelItem *)));

    showActiveElem();
    registrateTray();

//    AudioPlayer player;
////    qDebug() << "BPM " << player.getBpmValue(QUrl::fromLocalFile("C:/Users/JB/Desktop/Dj Maze feat. Dina Rae - Falling In Love.mp3")); // ~70
////    qDebug() << "BPM " << player.getBpmValue(QUrl::fromLocalFile("F:/katy_perry_-_last_friday_night_(zaycev.net).mp3")); // ~126
////    qDebug() << "BPM " << player.getBpmValue(QUrl::fromLocalFile("F:/katy_perry_feat._kanye_west_-_e.t._(zaycev.net).mp3")); // ~76
////    qDebug() << "BPM " << player.getBpmValue(QUrl::fromLocalFile("F:/Shakra – Trapped.mp3")); // ~105
////    qDebug() << "BPM " << player.getBpmValue(QUrl::fromLocalFile("F:/Yellow Claw feat. Rochelle - Shotgun .mp3")); // ~145
//    player.setMedia(QUrl::fromLocalFile("C:/Users/JB/Desktop/Akon_Ft_French_Montana_-_Hurt_Somebody.mp3"));
//    player.play();
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

    settings -> write("vk", VkApi::instance() -> toJson());
    settings -> write("soundcloud", SoundcloudApi::instance() -> toJson());

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
            curr_tab.insert("movable", bar -> isMovable());

            if (bar -> windowTitle() != "Media"
                    && bar -> windowTitle() != "Media+"
                    && bar -> windowTitle() != "Media+Position"
                    && bar -> windowTitle() != "Media+Time"
                    && bar -> windowTitle() != "Media+Volume"
                    && bar -> windowTitle() != "Controls"
                    && bar -> windowTitle() != "Spectrum"
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
        Genre::close();

        VkApi::close();
        SoundcloudApi::close();
        ToolBars::close();
    ///////////////////////////////////////////////

    delete settings;
    delete tabber;
}

QMenu * MainWindow::createPopupMenu () {
    return ToolBars::instance(this) -> improvePopupMenu(this, QMainWindow::createPopupMenu());
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

void MainWindow::openFolderTriggered() {
    ToolbarButton * button = (ToolbarButton*)QObject::sender();
    QDesktopServices::openUrl(QUrl::fromLocalFile(button -> path));
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
            tabber -> updateIconSize();

        ui -> tabber -> setTabPosition((QTabWidget::TabPosition)Settings::instance() -> getTabPosition());
        ui -> tabber -> setUsesScrollButtons(Settings::instance() -> getScrollButtonUsage());
        ToolBars::instance(this) -> getSpectrum() -> bandCountChanged(Settings::instance() -> getSpectrumBarsCount());
        ToolBars::instance(this) -> getSpectrum()  -> heightChanged(Settings::instance() -> getSpectrumHeight());
        Player::instance() -> setSpectrumFreq(Settings::instance() -> getSpectrumFreqRate());
    }
}

void MainWindow::showVKRelTabDialog() {
    RelationDialog dialog(VkApi::instance(), this);
    if (dialog.exec() == QDialog::Accepted) {
        tabber -> addTab("VK [" + dialog.getName() + "]", TabDialog::VKSettings(dialog.getId()));
    } else {
//        QMessageBox::information(this, "VK", VkApi::instance() -> getError());
    }
}

void MainWindow::showSoundcloudRelTabDialog() {
    RelationDialog dialog(SoundcloudApi::instance(), this);
    if (dialog.exec() == QDialog::Accepted) {
        tabber -> addTab("Soundcloud [" + dialog.getName() + "]", TabDialog::soundcloudSettings(dialog.getId()));
    } else {
//        QMessageBox::information(this, "VK", VkApi::instance() -> getError());
    }
}

void MainWindow::showVKTabDialog() {
    if (VkApi::instance() -> isConnected()) {
        tabber -> addTab("VK [YOU]", TabDialog::VKSettings());
    } else {
        WebDialog dialog(this, VkApi::instance(), "VK auth");
        if (dialog.exec() == QDialog::Accepted) {
            tabber -> addTab("VK [YOU]", TabDialog::VKSettings());
            ToolBars::instance(this) -> initiateVkButton();
        } else {
            QMessageBox::information(this, "VK", VkApi::instance() -> getError());
        }
    }
}

void MainWindow::showSoundcloudTabDialog() {
    if (SoundcloudApi::instance() -> isConnected()) {
        tabber -> addTab("Soundcloud", TabDialog::soundcloudSettings());
    } else {
        WebDialog dialog(this, SoundcloudApi::instance(), "Soundcloud auth");
        if (dialog.exec() == QDialog::Accepted) {
            tabber -> addTab("Soundcloud", TabDialog::soundcloudSettings());
            ToolBars::instance(this) -> initiateSoundcloudButton();
        } else {
            QMessageBox::information(this, "Soundcloud", SoundcloudApi::instance() -> getError());
        }
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
              if (ToolBars::instance(this) -> isToolbarNameUniq(this, dialog.getName())) {
                  tab -> setName(dialog.getName());
                  tab -> getList() -> setSettings(dialog.getSettings());
                  return;
              }
          } else return;
      }
  } else {
      while(true) {
          if (dialog.exec() == QDialog::Accepted) {
              if (ToolBars::instance(this) -> isToolbarNameUniq(this, dialog.getName())) {
                  tabber -> addTab(dialog.getName(), dialog.getSettings());
                  return;
              }
          } else return;
      }
  }
}

void MainWindow::showAttCurrTabDialog() {
//    emit showAttTabDialog(tabber -> currentTab());

    if (tabber -> currentTab() -> isEditable())
        emit showAttTabDialog(tabber -> currentTab());
    else
        QMessageBox::warning(this, "Settings", "This tab type did not have any settings...");
}
