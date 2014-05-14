#include "tabdialog.h"
#include "ui_tabdialog.h"

CBHash TabDialog::VKSettings() {
    CBHash res;
    res.insert("d", 0);
    res.insert("i", 0);
    res.insert("p", 1);
    res.insert("t", 4);

    return res;
}

TabDialog::TabDialog(QWidget *parent) :
  QDialog(parent), ui(new Ui::TabDialog) {
  ui->setupUi(this);

  setWindowTitle("Tab settings");
  ui -> interactive -> setEnabled(false);
}

TabDialog::~TabDialog() {
  delete ui;
}

QString TabDialog::getName() {
  return ui ->tabName ->text();
}

void TabDialog::setName(QString name) {
  ui -> tabName -> setText(name);
}

CBHash TabDialog::getSettings() {
  CBHash res;
  res.insert("d", ui -> deleteFile -> checkState() == Qt::Checked ? 1 : 0);
  res.insert("i", ui -> interactive -> checkState() == Qt::Checked ? 1 : 0);
  res.insert("p", ui -> playlist -> checkState() == Qt::Checked ? 1 : 0);

  if (ui -> isListRadio -> isChecked())
      res.insert("t", 1);
  else if (ui -> IsOneLevelTreeRadio -> isChecked())
      res.insert("t", 2);
  else if (ui -> isTreeRadio -> isChecked())
      res.insert("t", 3);
  else res.insert("t", 4);

  return res;
}

void TabDialog::setSettings(CBHash hash) {
  ui -> deleteFile -> setChecked(hash.value("d") == 1);
  ui -> interactive -> setChecked(hash.value("i") == 1);
  ui -> playlist -> setChecked(hash.value("p") == 1);

  ui -> isListRadio -> setChecked(hash.value("t") == 1);
  ui -> IsOneLevelTreeRadio -> setChecked(hash.value("t") == 2);
  ui -> isTreeRadio -> setChecked(hash.value("t") == 3);

  ui -> isListRadio -> setEnabled(false);
  ui -> isTreeRadio -> setEnabled(false);
  ui -> IsOneLevelTreeRadio -> setEnabled(false);
}
