#ifndef VKRELATIONDIALOG_H
#define VKRELATIONDIALOG_H

#include <QDialog>
#include <QStringListModel>

#include "web/socials/vk_api.h"

namespace Ui {
class VkRelationDialog;
}

class VkRelationDialog : public QDialog {
    Q_OBJECT

public:
    explicit VkRelationDialog(QWidget *parent = 0);
    ~VkRelationDialog();

    int getId() const;

private slots:
    void on_cancelButton_clicked();

    void on_friendsList_activated(const QModelIndex &index);

    void on_groupsList_activated(const QModelIndex &index);

private:
    Ui::VkRelationDialog *ui;
    int uid;
    QString name;
    QStringListModel * friendModel, *groupModel;
};

#endif // VKRELATIONDIALOG_H
