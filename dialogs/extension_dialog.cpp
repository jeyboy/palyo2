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
    ui -> presetExtensions -> model() -> removeRows(0, 255);
    ui -> presetExtensions -> addItems(Extensions::instance() -> filterList(name));
}

void ExtensionDialog::on_addExtension_clicked() {

}
