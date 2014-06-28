#ifndef CAPTCHADIALOG_H
#define CAPTCHADIALOG_H

#include <QDialog>
#include <QPicture>
#include "web/web_api.h"

namespace Ui {
class CaptchaDialog;
}

class CaptchaDialog : public QDialog {
    Q_OBJECT

public:
    explicit CaptchaDialog(WebApi * api, QWidget *parent = 0);
    ~CaptchaDialog();
    void setImage(QString url);
    QString captchaText() const;

private:
    Ui::CaptchaDialog *ui;
    WebApi * api;
};

#endif // CAPTCHADIALOG_H
