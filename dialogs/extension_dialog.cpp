#include "extension_dialog.h"
#include "ui_extension_dialog.h"
#include "misc/extensions.h"

#include <QStringListModel>

ExtensionDialog::ExtensionDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::ExtensionDialog) {
    ui -> setupUi(this);

    ui -> presets -> addItems(Extensions::instance() -> presetsList());
    ui -> presetExtensions -> setModel(new QStringListModel(Extensions::instance() -> activeFilterList(), this));
}

ExtensionDialog::~ExtensionDialog() {
    delete ui;
}

void ExtensionDialog::on_addPreset_clicked() {

}

void ExtensionDialog::on_presets_currentIndexChanged(const QString & name) {
    ((QStringListModel *)ui -> presetExtensions -> model()) -> setStringList(Extensions::instance() -> filterList(name));
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

    Extensions::instance() -> filterListUpdate(ui -> presets -> currentText(), list);
}

void ExtensionDialog::proceedFilter(QString & filter, QStringList & preset) {
    if (!filter.startsWith("*."))
        filter = "*." + filter;

    if (!preset.contains(filter))
        preset.append(preset);
}
