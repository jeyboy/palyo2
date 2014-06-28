#include "captcha.h"

Captcha::Captcha() : QObject() {

}

QString Captcha::showDialog(WebApi * api, QString captchaUrl) {
    CaptchaDialog dialog(api); // parent needed
    dialog.setImage(captchaUrl);
    if (dialog.exec() == QDialog::Accepted) {
        return dialog.captchaText();
    }

    return "";
}
