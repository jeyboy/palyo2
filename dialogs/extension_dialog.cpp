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
}

ExtensionDialog::~ExtensionDialog() {
    delete ui;
}

void ExtensionDialog::on_addPreset_clicked() {

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

void ExtensionDialog::proceedFilter(QString & filter, QStringList & preset) {
    if (!filter.startsWith("*."))
        filter = "*." + filter;

    if (!preset.contains(filter))
        preset.append(filter);
}
