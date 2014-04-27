#include "web_dialog.h"
#include "ui_web_dialog.h"
#include <qDebug>

WebDialog::WebDialog(QWidget *parent, QString appName, QString title, QString url) :
  QDialog(parent), ui(new Ui::WebDialog) {
  ui->setupUi(this);

  setWindowTitle(title);
  app_name = appName;

  CustomNetworkAccessManager *networkAccessManager = new CustomNetworkAccessManager(QSsl::TlsV1SslV3, QSslSocket::VerifyNone);
  QWebView* view = ui -> webView;
  view -> page() -> setNetworkAccessManager(networkAccessManager);

  connect(view, SIGNAL(urlChanged(const QUrl&)), SLOT(urlChanged(const QUrl&)));

  view -> load(QUrl(url));
  view -> show();
}

WebDialog::~WebDialog() {
  delete ui;
}

QString WebDialog::getError() {
    return error;
}
QString WebDialog::getToken() {
    return token;
}
QString WebDialog::getExpire() {
    return expires_in;
}
QString WebDialog::getUserID() {
    return user_id;
}

////////////////////////////////////////////////////////////
/// SLOTS
////////////////////////////////////////////////////////////

void WebDialog::urlChanged(const QUrl& url) {
    if (app_name == "vk") {
        vkResponse(url);
    } else {
        reject();
    }
}

////////////////////////////////////////////////////////////
/// PRIVATE
////////////////////////////////////////////////////////////

void WebDialog::vkResponse(const QUrl& url) {
    QUrlQuery query(url.fragment());

    if (query.hasQueryItem("error")) {
        error = query.queryItemValue("error_description");
        reject();
    } else if (query.hasQueryItem("access_token")) {\
        token = query.queryItemValue("access_token");
        expires_in = query.queryItemValue("expires_in");
        user_id = query.queryItemValue("user_id");
        accept();
    }
}
