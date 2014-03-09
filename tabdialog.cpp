#include "tabdialog.h"
#include "ui_tabdialog.h"

TabDialog::TabDialog(QWidget *parent) :
  QDialog(parent), ui(new Ui::TabDialog) {
  ui->setupUi(this);
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

//TODO: add check on name uniqueness

CBHash TabDialog::getSettings() {
  CBHash res;
  res.insert("d", ui -> deleteFile -> checkState() == Qt::Checked);
  res.insert("i", ui -> interactive -> checkState() == Qt::Checked);
  res.insert("p", ui -> playlist -> checkState() == Qt::Checked);
  return res;
}

void TabDialog::setSettings(CBHash & hash) {
  ui -> deleteFile -> setChecked(hash.value("d"));
  ui -> interactive -> setChecked(hash.value("i"));
  ui -> playlist -> setChecked(hash.value("p"));
}
