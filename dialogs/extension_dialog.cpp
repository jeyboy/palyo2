#include "extension_dialog.h"
#include "ui_extension_dialog.h"
#include "misc/extensions.h"
#include <QDebug>

#include <QStringListModel>

ExtensionDialog::ExtensionDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::ExtensionDialog) {
    ui -> setupUi(this);

    ui -> presetExtensions -> setModel(new QStringListModel(Extensions::instance() -> activeFilterList(), this));
    ui -> presets -> addItems(Extensions::instance() -> presetsList());
    ui -> presets -> setCurrentIndex(ui -> presets -> findText(Extensions::instance() -> activeFilterName()));

    ui -> newPresetName -> setVisible(false);
    ui -> addPreset -> setVisible(false);
    ui -> cancelPreset -> setVisible(false);
}

ExtensionDialog::~ExtensionDialog() {
    delete ui;
}

void ExtensionDialog::on_presets_currentIndexChanged(const QString & name) {
    Extensions::instance() -> setActiveFilterName(name);
    ((QStringListModel *)ui -> presetExtensions -> model()) -> setStringList(Extensions::instance() -> activeFilterList());
}

void ExtensionDialog::on_addExtension_clicked() {
    QStringList list = ((QStringListModel *)ui -> presetExtensions -> model()) -> stringList();
    QString newPreset = ui -> extension -> text().replace(' ', "").toLower();
    if (newPreset.isEmpty()) return;

    if (newPreset.contains(',')) {
        QStringList newPresets = newPreset.split(',', QString::SkipEmptyParts);
        foreach(QString name, newPresets)
            proceedFilter(name, list);
    } else {
        proceedFilter(newPreset, list);
    }

    ui -> extension -> setText("");
    Extensions::instance() -> filterListUpdate(ui -> presets -> currentText(), list);
    ((QStringListModel *)ui -> presetExtensions -> model()) -> setStringList(list);
}

void ExtensionDialog::on_removeFilter_clicked() {
    foreach(QModelIndex index, ui -> presetExtensions -> selectedIndexes()) {
//        index.data()
    }
}

void ExtensionDialog::proceedFilter(QString & filter, QStringList & preset) {
    if (!filter.startsWith("*."))
        filter = "*." + filter;

    if (!preset.contains(filter))
        preset.append(filter);
}

void ExtensionDialog::on_newPreset_clicked() {
    ui -> newPresetName -> setVisible(true);
    ui -> addPreset -> setVisible(true);
    ui -> cancelPreset -> setVisible(true);
}

void ExtensionDialog::on_removePreset_clicked() {

}

void ExtensionDialog::on_addPreset_clicked() {
    Extensions::instance() -> addNewPreset(ui -> newPresetName -> text());
    ui -> presets -> clear();
    Extensions::instance() -> setActiveFilterName(ui -> newPresetName -> text());
    ui -> presets -> addItems(Extensions::instance() -> presetsList());
    ui -> presets -> setCurrentIndex(ui -> presets -> findText(Extensions::instance() -> activeFilterName()));

    ui -> newPresetName -> setVisible(false);
    ui -> addPreset -> setVisible(false);
    ui -> cancelPreset -> setVisible(false);
    ui -> newPresetName -> setText("");
}

void ExtensionDialog::on_cancelPreset_clicked() {
    ui -> newPresetName -> setVisible(false);
    ui -> addPreset -> setVisible(false);
    ui -> cancelPreset -> setVisible(false);
    ui -> newPresetName -> setText("");
}
