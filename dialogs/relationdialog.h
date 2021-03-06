#ifndef RELATIONDIALOG_H
#define RELATIONDIALOG_H

#include <QDialog>
#include <QStringListModel>

#include "web/web_api.h"

namespace Ui {
class RelationDialog;
}

class RelationDialog : public QDialog {
    Q_OBJECT

public:
    explicit RelationDialog(WebApi * currApi, QWidget *parent = 0);
    ~RelationDialog();

    int getId() const;
    QString getName();

private slots:
    void on_cancelButton_clicked();

    void on_friendsList_activated(const QModelIndex &index);

    void on_groupsList_activated(const QModelIndex &index);

private:
    Ui::RelationDialog *ui;
    int uid;
    QString name;
    QStringListModel * friendModel, *groupModel;
    WebApi * api;
};

#endif // RELATIONDIALOG_H
