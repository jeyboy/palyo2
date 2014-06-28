#ifndef CAPTCHA_H
#define CAPTCHA_H

#include <QObject>
#include "dialogs/captchadialog.h"

class Captcha : public QObject {
    Q_OBJECT
public:
    Captcha();
    QString showDialog(WebApi * api, QString captchaUrl);
};

#endif // CAPTCHA_H
