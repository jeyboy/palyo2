#include "mainwindow.h"
#include "single_application.h"
#include <qDebug>

int main(int argc, char *argv[]) {
    qRegisterMetaType<QVector <int> >("QVector<int>");

    QCoreApplication::setOrganizationName("BigBug");
    QCoreApplication::setOrganizationDomain("bigbug.sos");
    QCoreApplication::setApplicationName("Playo");

    SingleApplication app(argc, argv, "bigbugplayo");

    if (app.isRunning()) {
        QStringList list = QCoreApplication::arguments();
        if (list.length() > 1) {
            list.removeFirst();
            app.sendMessage(list.join('|'));
        }
        return 0;
    }

    MainWindow * mainWindow = new MainWindow();
    QObject::connect(&app, SIGNAL(messageAvailable(QString)), mainWindow, SLOT(receiveMessage(QString)));
    mainWindow -> show();

    return app.exec();
}
