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
