#include "captchadialog.h"
#include "ui_captchadialog.h"

CaptchaDialog::CaptchaDialog(QWidget * parent) :
    QDialog(parent), ui(new Ui::CaptchaDialog) {
    ui->setupUi(this);
}

CaptchaDialog::~CaptchaDialog() {
    delete ui;
}

void CaptchaDialog::setImage(WebApi * api, QString url) {
    QImage image;
    image.loadFromData(api -> openRemoteImage(url));
    ui -> captchaImg -> setPixmap(QPixmap::fromImage(image));
}

QString CaptchaDialog::captchaText() const {
    return ui -> captchaText -> text();
}

void CaptchaDialog::on_buttonBox_rejected() {
    ui -> captchaText -> setText("");
}
