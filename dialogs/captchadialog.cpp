#include "captchadialog.h"
#include "ui_captchadialog.h"

CaptchaDialog::CaptchaDialog(WebApi * api, QWidget * parent) :
    QDialog(parent), ui(new Ui::CaptchaDialog) {
    ui->setupUi(this);

    this -> api = api;
}

CaptchaDialog::~CaptchaDialog() {
    delete ui;
}

void CaptchaDialog::setImage(QString url) {
    QImage image;
    image.loadFromData(api -> openRemoteImage(url));
    ui -> captchaImg -> setPixmap(QPixmap::fromImage(image));
}

QString CaptchaDialog::captchaText() const {
    return ui -> captchaText -> text();
}
