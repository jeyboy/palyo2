#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "misc/screen.h"
#include "media/media_player/utils/system_utils.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow) {
    ui -> setupUi(this);

    QApplication::setWindowIcon(QIcon(":icon"));
    setWindowTitle("Playo");
    setAcceptDrops(true);
    setAttribute(Qt::WA_DeleteOnClose);

    initialization();

    player = new MediaPlayer(this);
    connect(player, SIGNAL(errorOccured(QString)), this, SLOT(showError(QString)));

//    Play media from Internet services using the quvi project.
//    The demuxer accepts a ‘format’ option to request a specific quality. It is by default set to best.
//    See http://quvi.sourceforge.net/ for more information.
//    FFmpeg needs to be built with --enable-libquvi for this demuxer to be enabled.
//    http://quvi.sourceforge.net/r/howto/install/
//    To play/convert videos directly from Youtube, using FFmpeg, you need to compile your ffmpeg with ​libquvi support.
//    player -> open(QUrl("http://www.youtube.com/watch?v=dQw4w9WgXcQ"));

      player -> openMillis(QUrl::fromLocalFile("G:/Million.Sposobov.Poteryat'.Golovu.2014.RUS.BDRip.x264.-HELLYWOOD.mkv"), 38000);
//      player -> openMillis(QUrl::fromLocalFile("L:/VIDEO/Dick Figures The Movie.mp4"), 60000);
//    player -> open(QUrl::fromLocalFile("G:/test/test3/New Year, New Tricks - Happy 2014!.mkv"));
//    player -> open(QUrl("http://www.ex.ua/get/111412158")); //short
//    player -> open(QUrl("http://www.ex.ua/get/120031676")); //asterix

//    player -> open(QUrl("http://www.ex.ua/get/47935441"));

//    player -> open(QUrl("http://www.ex.ua/get/47933391"));



    player -> open(QUrl::fromLocalFile("F:/Muzon/feel u.wma"));
//    player -> open(QUrl::fromLocalFile("G:/test/test3/Стрелок (RUS).mp4"));
//    qDebug() << player -> open(QUrl("http://www.ex.ua/get/116023228")); //asterix 2
//    qDebug() << player -> open(QUrl("https://cs1-43v4.vk-cdn.net/p12/f97b321ed3ed8a.mp3?extra=b8mi7AY_T_njmTpycZLbdJnNCoI1hgzvLkAJIA83H16rdNecFzZqMVHLtCcj7liPcl0CnVn7xFTDKEkx7xDG_FSznd8nUZ3S"));
//    player -> open(QUrl::fromLocalFile("G:/test/FlameMakers - Álmok szigetén (Club Hupák Remix).mp3"));




//    player -> open(QUrl::fromLocalFile("G:/test/test3/test.mp4"));
//    player -> open(QUrl::fromLocalFile("G:/test/test3/test.mp3"));
//      player -> open(QUrl::fromLocalFile("G:/test/test3/not played.mp3"));
//      player -> open(QUrl::fromLocalFile("G:/test/test3/Shakra – Trapped.mp3"));
//      player -> open(QUrl::fromLocalFile("G:/test/test3/Белоснежка и брачный сезон.mp4"));
//      player -> open(QUrl::fromLocalFile("F:/multichannel test/01. Please Don't Keep Me Waiting.wav"));


//    MediaAttributes * attrs = 0;
//    attrs = player -> getInfo(QUrl::fromLocalFile("G:/test/test3/Стрелок (RUS).mp4"));
////    qDebug() << attrs -> operator QString();
////    qDebug() << "-------------------------";
//    delete attrs;

//    attrs = player -> getInfo(QUrl::fromLocalFile("G:/test/test3/test.mp3"));
////    qDebug() << attrs -> operator QString();
////    qDebug() << "-------------------------";
//    delete attrs;

//    attrs = player -> getInfo(QUrl("http://www.ex.ua/get/116023228"));
////    qDebug() << attrs -> operator QString();
////    qDebug() << "-------------------------";
//    delete attrs;

    player -> play();
}

void MainWindow::locationCorrection() {
    int width, height;
    Screen::screenSize(width, height);
    int left = x(), top = y();

    if (left >= width)
        left = width - 50;

    if (top >= height)
        top = height - 50;

    move(left, top);
}

void MainWindow::initialization() {
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
    locationCorrection();
    ///////////////////////////////////////////////////////////
    /// toolbars
    ///////////////////////////////////////////////////////////
    QJsonArray bars = settings -> read("bars").toArray();
    ToolBars::instance(this) -> load(this, bars);

    QVariant objState = stateSettings.value("windowState");
    if (objState.isValid())
        restoreState(objState.toByteArray());
    ///////////////////////////////////////////////////////////
    ui -> tabber -> load();

    connect(Player::instance(), SIGNAL(itemChanged(ModelItem *, ModelItem *)), this, SLOT(outputActiveItem(ModelItem *, ModelItem *)));

    showActiveElem();
    registrateTray();
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
    return 0;
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

    ToolBars::instance(this) -> save(this, settings);

    settings -> write("settings", Settings::instance() -> toJson());
    settings -> save();

    QSettings stateSettings("settings.ini", QSettings::IniFormat, this);
    stateSettings.setValue("geometry", saveGeometry());
    stateSettings.setValue("windowState", saveState());
    stateSettings.sync();

    ui -> tabber -> save();

    m_tray.hide();
    event -> accept();
//    QApplication::quit();
}

MainWindow::~MainWindow() {
    delete ui;

    ///////////////////////////////////////////////
    /// close singletons
    ///////////////////////////////////////////////

        Extensions::close();
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
}

QMenu * MainWindow::createPopupMenu () {
    return ToolBars::instance(this) -> improvePopupMenu(this, QMainWindow::createPopupMenu());
}

/////////////////////////////////////////////////////////////////////////////////////
///SLOTS
/////////////////////////////////////////////////////////////////////////////////////

void MainWindow::showError(QString message) {
    qDebug() << message;
}

void MainWindow::nextItemTriggered() {
    if (ui -> tabber -> currentTab())
        ui -> tabber -> currentTab() -> getView() -> proceedNext();
}
void MainWindow::nextItemWithDelTriggered() {
    if (ui -> tabber -> currentTab())
        ui -> tabber -> currentTab() -> getView() -> deleteCurrentProceedNext();
}
void MainWindow::prevItemTriggered() {
    if (ui -> tabber -> currentTab())
        ui -> tabber -> currentTab() -> getView() -> proceedPrev();
}

void MainWindow::openFolderTriggered() {
    ToolbarButton * button = (ToolbarButton*)QObject::sender();
    QDesktopServices::openUrl(QUrl::fromLocalFile(button -> path));
}

void MainWindow::showActiveElem() {
    Tab * tab = ui -> tabber -> toActiveTab();
    if (tab)
        tab -> getView() -> scrollToActive();
}

void MainWindow::showSettingsDialog() {
    SettingsDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        if (dialog.isIconSizeChanged())
            ui -> tabber -> updateIconSize();

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
        ui -> tabber -> addTab("VK [" + dialog.getName() + "]", TabDialog::VKSettings(dialog.getId()));
    } else {
//        QMessageBox::information(this, "VK", VkApi::instance() -> getError());
    }
}

void MainWindow::showSoundcloudRelTabDialog() {
    RelationDialog dialog(SoundcloudApi::instance(), this);
    if (dialog.exec() == QDialog::Accepted) {
        ui -> tabber -> addTab("SC [" + dialog.getName() + "]", TabDialog::soundcloudSettings(dialog.getId()));
    } else {
//        QMessageBox::information(this, "VK", VkApi::instance() -> getError());
    }
}

void MainWindow::openVKTabDialog() {
    WebDialog dialog(this, VkApi::instance(), "VK auth");
    if (dialog.exec() == QDialog::Accepted) {
        ui -> tabber -> addTab("VK [YOU]", TabDialog::VKSettings());
        ToolBars::instance(this) -> initiateVkButton();
    } else {
        QMessageBox::information(this, "VK", VkApi::instance() -> getError());
    }
}

void MainWindow::showVKTabDialog() {
    if (VkApi::instance() -> isConnected()) {
        ui -> tabber -> addTab("VK [YOU]", TabDialog::VKSettings());
    } else openVKTabDialog();
}

void MainWindow::openSoundcloudTabDialog() {
    WebDialog dialog(this, SoundcloudApi::instance(), "Soundcloud auth");
    if (dialog.exec() == QDialog::Accepted) {
        ui -> tabber -> addTab("SC [YOU]", TabDialog::soundcloudSettings());
        ToolBars::instance(this) -> initiateSoundcloudButton();
    } else {
        QMessageBox::information(this, "Soundcloud", SoundcloudApi::instance() -> getError());
    }
}

void MainWindow::showSoundcloudTabDialog() {
    if (SoundcloudApi::instance() -> isConnected()) {
        ui -> tabber -> addTab("SC [YOU]", TabDialog::soundcloudSettings());
    } else openSoundcloudTabDialog();
}

void MainWindow::outputActiveItem(ModelItem *, ModelItem * to) {
    if (to && !this -> isActiveWindow())
        m_tray.showMessage("(" + QString::number(ui -> tabber -> currentTab() -> getView() -> itemsCount()) + ") Now played:", to -> data(TITLEID).toString(), QSystemTrayIcon::Information, 20000);
}

void MainWindow::putToCommonTab(QList<QUrl> urls) {
    ui -> tabber -> commonTab() -> getView() -> getModel() -> dropProcession(urls);
    ui -> tabber -> commonTab() -> getView() -> getModel() -> refresh();

    if (!Player::instance() -> isPlayed()) {
        ui -> tabber -> commonTab() -> getView() -> proceedNext();
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
      dialog.setSettings(tab -> getView() -> getSettings());
      dialog.setName(tab -> getName());

      while(true) {
          if (dialog.exec() == QDialog::Accepted) {
              if (ToolBars::instance(this) -> isToolbarNameUniq(this, dialog.getName())) {
                  tab -> setName(dialog.getName());
                  tab -> getView() -> setSettings(dialog.getSettings());
                  return;
              }
          } else return;
      }
  } else {
      while(true) {
          if (dialog.exec() == QDialog::Accepted) {
              if (ToolBars::instance(this) -> isToolbarNameUniq(this, dialog.getName())) {
                  ui -> tabber -> addTab(dialog.getName(), dialog.getSettings());
                  return;
              }
          } else return;
      }
  }
}

void MainWindow::showAttCurrTabDialog() {
//    emit showAttTabDialog(tabber -> currentTab());

    if (ui -> tabber -> currentTab() -> isEditable())
        emit showAttTabDialog(ui -> tabber -> currentTab());
    else
        QMessageBox::warning(this, "Settings", "This tab type did not have any settings...");
}
