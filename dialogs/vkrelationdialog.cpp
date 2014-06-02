#include "vkrelationdialog.h"
#include "ui_vkrelationdialog.h"
#include <qDebug>

VkRelationDialog::VkRelationDialog(QWidget *parent) : QDialog(parent), ui(new Ui::VkRelationDialog) {
    ui->setupUi(this);

    friendModel = new QStringListModel(VkApi::instance() -> friendsList().values());
    ui -> friendsList -> setModel(friendModel);
    friendModel -> sort(0);

    groupModel = new QStringListModel(VkApi::instance() -> groupsList().values());
    ui -> groupsList -> setModel(groupModel);
    groupModel -> sort(0);

    ui -> friendsList -> setEditTriggers(QListView::NoEditTriggers);
    ui -> groupsList -> setEditTriggers(QListView::NoEditTriggers);
}

VkRelationDialog::~VkRelationDialog() {
    delete ui;
    delete friendModel;
    delete groupModel;
}

int VkRelationDialog::getId() const {
    return uid;
}

QString VkRelationDialog::getName() {
    return name;
}

void VkRelationDialog::on_cancelButton_clicked() {
    reject();
}

void VkRelationDialog::on_friendsList_activated(const QModelIndex &index) {
    name = index.data().toString();
    uid = VkApi::instance() -> friendsList().key(name).toInt();
    accept();
}

void VkRelationDialog::on_groupsList_activated(const QModelIndex &index) {
    name = index.data().toString();
    uid = VkApi::instance() -> groupsList().key(name).toInt() * -1;
    accept();
}
