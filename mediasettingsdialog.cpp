#include "mediasettingsdialog.h"
#include "ui_mediasettingsdialog.h"

MediaSettingsDialog::MediaSettingsDialog(QWidget *parent) :
  QDialog(parent), ui(new Ui::MediaSettingsDialog) {
  ui->setupUi(this);

  setWindowTitle("Media settings");
}

MediaSettingsDialog::~MediaSettingsDialog() {
  delete ui;
}

void MediaSettingsDialog::on_closeButton_clicked() {
    close();
}
