#include "web_dialog.h"
#include "ui_web_dialog.h"
#include <qDebug>

WebDialog::WebDialog(QWidget *parent, WebApi * apiClass, QString title) :
  QDialog(parent), ui(new Ui::WebDialog) {
  ui->setupUi(this);

  setWindowTitle(title);
  api = apiClass;

  CustomNetworkAccessManager *networkAccessManager = new CustomNetworkAccessManager(QSsl::TlsV1SslV3, QSslSocket::VerifyNone);
  QWebView* view = ui -> webView;
  view -> page() -> setNetworkAccessManager(networkAccessManager);

  connect(view, SIGNAL(urlChanged(const QUrl&)), SLOT(urlChanged(const QUrl&)));

  view -> load(QUrl(api -> authUrl()));
  view -> show();
}

WebDialog::~WebDialog() {
  delete ui;
}

////////////////////////////////////////////////////////////
/// SLOTS
////////////////////////////////////////////////////////////

void WebDialog::urlChanged(const QUrl& url) {
    QString res = api -> proceedResponse(url);
    qDebug() << "Hula: " << res;
    if (res == "accept") {
        accept();
    } else if (res == "reject") {
        reject();
    } else if (res.length() > 0) {
        QWebView* view = ui -> webView;
        view -> load(QUrl(res));
    }
}