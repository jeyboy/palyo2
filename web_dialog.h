#ifndef WEB_DIALOG_H
#define WEB_DIALOG_H

#include <QDialog>
#include <QtWebKitWidgets>
#include "custom_network_access_manager.h"

namespace Ui {
    class WebDialog;
}

class WebDialog : public QDialog {
    Q_OBJECT

public:
    explicit WebDialog(QWidget *parent, QString appName, QString title, QString url);
    ~WebDialog();

    QString getError();
    QString getToken();
    QString getExpire();
    QString getUserID();

private slots:
    void urlChanged(const QUrl&);

private:
    void vkResponse(const QUrl& url);

    QString app_name;
    Ui::WebDialog *ui;

    QString error;
    QString token;
    QString expires_in;
    QString user_id;
};


#endif // WEB_DIALOG_H
