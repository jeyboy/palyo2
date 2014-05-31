#include "settings_dialog.h"
#include "ui_settings_dialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
  QDialog(parent), ui(new Ui::SettingsDialog) {
  ui -> setupUi(this);

  setWindowTitle("Settings ept");
  setFixedWidth(356);
  setFixedHeight(291);
  setSizeGripEnabled(false);

  ui -> treeView -> setEditTriggers(QTreeView::AllEditTriggers);
  ui -> treeView -> setItemDelegate(new HotkeyDelegate(ui -> treeView));
  ui -> treeView -> setModel(new HotkeyModel(Settings::instance() -> getHotKeys(), this));
  ui -> treeView -> hideColumn(2);
  ui -> treeView -> setColumnWidth(0, 250);

  ui -> downloadPath -> setText(Settings::instance() -> getDownloadPath());

  ui -> showCheckboxes -> setChecked(Settings::instance() -> isCheckboxShow());
  ui -> drawMetrics -> setChecked(Settings::instance() -> isMetricShow());
  ui -> spoilOnActivate -> setChecked(Settings::instance() -> isSpoilOnActivation());
  ui -> showInfo -> setChecked(Settings::instance() -> isShowInfo());

  ui -> useGradientCheck -> setChecked(Settings::instance() -> isUseGradient());

  defaultColor = Settings::instance() -> getDefaultColor();
  ui -> defaultColorButton -> setStyleSheet("background-color: " + defaultColor.name() + ";");

  listenedColor = Settings::instance() -> getListenedColor();
  ui -> listenedColorButton -> setStyleSheet("background-color: " + listenedColor.name() + ";");

  likedColor = Settings::instance() -> getLikedColor();
  ui -> likedColorButton -> setStyleSheet("background-color: " + likedColor.name() + ";");

  playedColor = Settings::instance() -> getPlayedColor();
  ui -> playedColorButton -> setStyleSheet("background-color: " + playedColor.name() + ";");

  folderColor = Settings::instance() -> getFolderColor();
  ui -> folderColorButton -> setStyleSheet("background-color: " + folderColor.name() + ";");
}

SettingsDialog::~SettingsDialog() {
  delete ui;
}

void SettingsDialog::registerHotkeys(QWidget * receiver) {
    QList<HotkeyModelItem *> list = *Settings::instance() -> getHotKeys();

    foreach(HotkeyModelItem * key, list) {
        switch(key -> data(2).toInt()) {
            case HOTKEY_NEXT: {
                HotkeyManager::instance() -> registerSequence(key -> data(2).toInt(), key -> data(1).toString(), receiver, SLOT(nextItemTriggered()));
            break;}
            case HOTKEY_NEXT_AND_DELETE: {
                HotkeyManager::instance() -> registerSequence(key -> data(2).toInt(), key -> data(1).toString(), receiver, SLOT(nextItemWithDelTriggered()));
            break;}
            case HOTKEY_PREV: {
                HotkeyManager::instance() -> registerSequence(key -> data(2).toInt(), key -> data(1).toString(), receiver, SLOT(prevItemTriggered()));
            break;}
            case HOTKEY_PLAY: {
                HotkeyManager::instance() -> registerSequence(key -> data(2).toInt(), key -> data(1).toString(), Player::instance(), SLOT(playPause()));
            break;}
            case HOTKEY_STOP: {
                HotkeyManager::instance() -> registerSequence(key -> data(2).toInt(), key -> data(1).toString(), Player::instance(), SLOT(stop()));
            break;}
            case HOTKEY_SETTINGS: {
                HotkeyManager::instance() -> registerSequence(key -> data(2).toInt(), key -> data(1).toString(), receiver, SLOT(showAttCurrTabDialog()));
            break;}


            case HOTKEY_POS_SLIDE_FORWARD: {
                HotkeyManager::instance() -> registerSequence(key -> data(2).toInt(), key -> data(1).toString(), Player::instance(), SLOT(slidePosForward()));
            break;}

            case HOTKEY_POS_SLIDE_BACKWARD: {
                HotkeyManager::instance() -> registerSequence(key -> data(2).toInt(), key -> data(1).toString(), Player::instance(), SLOT(slidePosBackward()));
            break;}

            case HOTKEY_VOL_SLIDE_FORWARD: {
                HotkeyManager::instance() -> registerSequence(key -> data(2).toInt(), key -> data(1).toString(), Player::instance(), SLOT(slideVolForward()));
            break;}

            case HOTKEY_VOL_SLIDE_BACKWARD: {
                HotkeyManager::instance() -> registerSequence(key -> data(2).toInt(), key -> data(1).toString(), Player::instance(), SLOT(slideVolBackward()));
            break;}
        }
    }
}


void SettingsDialog::on_cancelButton_clicked() {
    reject();
}

void SettingsDialog::on_acceptButton_clicked() {
    HotkeyModel * model = dynamic_cast<HotkeyModel *>(ui -> treeView -> model());
    QList<HotkeyModelItem *> list = model -> toplevelItems();
    Settings::instance() -> setHotKeys(list);

    HotkeyManager::instance() -> clear();

    foreach(HotkeyModelItem * key, list) {
        HotkeyManager::instance() -> registerSequence(key -> data(2).toInt(), key -> data(1).toString());
    }

    Settings::instance() -> setDownloadPath(ui -> downloadPath -> text());
    Settings::instance() -> setCheckboxShow(ui -> showCheckboxes -> isChecked());
    Settings::instance() -> setMetricShow(ui -> drawMetrics -> isChecked());

    Settings::instance() -> setSpoilOnActivation(ui -> spoilOnActivate -> isChecked());

    iconSizeChanged = Settings::instance() -> isShowInfo() != ui -> showInfo -> isChecked();
    Settings::instance() -> setShowInfo(ui -> showInfo -> isChecked());

    Settings::instance() -> setUseGradient(ui -> useGradientCheck -> isChecked());

    Settings::instance() -> setDefaultColor(defaultColor);
    Settings::instance() -> setListenedColor(listenedColor);
    Settings::instance() -> setLikedColor(likedColor);
    Settings::instance() -> setPlayedColor(playedColor);
    Settings::instance() -> setFolderColor(folderColor);

    accept();
}

void SettingsDialog::on_browseButton_clicked() {
    QString path = QFileDialog::getExistingDirectory(this, "Please choose new download path");
    if (!path.isEmpty()) {
        if (!path.endsWith('/'))
            path += "/";
        ui -> downloadPath -> setText(path);
    }
}

bool SettingsDialog::isIconSizeChanged() const {
    return iconSizeChanged;
}
bool SettingsDialog::isBigIcon() const {
    return ui -> showInfo -> isChecked();
}


void SettingsDialog::on_defaultColorButton_clicked() {
    QColorDialog col(this);
    col.setCurrentColor(defaultColor);
    if (col.exec() == QColorDialog::Accepted) {
        defaultColor = col.selectedColor();
        ui -> defaultColorButton -> setStyleSheet("background-color: " + defaultColor.name() + ";");
    }
}

void SettingsDialog::on_listenedColorButton_clicked() {
    QColorDialog col(this);
    col.setCurrentColor(listenedColor);
    if (col.exec() == QColorDialog::Accepted) {
        listenedColor = col.selectedColor();
        ui -> listenedColorButton -> setStyleSheet("background-color: " + listenedColor.name() + ";");
    }
}

void SettingsDialog::on_likedColorButton_clicked() {
    QColorDialog col(this);
    col.setCurrentColor(likedColor);
    if (col.exec() == QColorDialog::Accepted) {
        likedColor = col.selectedColor();
        ui -> likedColorButton -> setStyleSheet("background-color: " + likedColor.name() + ";");
    }
}

void SettingsDialog::on_playedColorButton_clicked() {
    QColorDialog col(this);
    col.setCurrentColor(playedColor);
    if (col.exec() == QColorDialog::Accepted) {
        playedColor = col.selectedColor();
        ui -> playedColorButton -> setStyleSheet("background-color: " + playedColor.name() + ";");
    }
}

void SettingsDialog::on_folderColorButton_clicked() {
    QColorDialog col(this);
    col.setCurrentColor(folderColor);
    if (col.exec() == QColorDialog::Accepted) {
        folderColor = col.selectedColor();
        ui -> folderColorButton -> setStyleSheet("background-color: " + folderColor.name() + ";");
    }
}
