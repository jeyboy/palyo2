#include "mainwindow.h"
#include "single_application.h"
#include <qDebug>

int main(int argc, char *argv[]) {
    qRegisterMetaType<QVector <int> >("QVector<int>");

    QCoreApplication::setOrganizationName("BigBug");
    QCoreApplication::setOrganizationDomain("bigbug.sos");
    QCoreApplication::setApplicationName("Playo");

    QCoreApplication::addLibraryPath("platforms");

    SingleApplication app(argc, argv, "bigbugplayo");

    QString message;
    QStringList list = QCoreApplication::arguments();
    if (list.length() > 1) {
        list.removeFirst();
        message = list.join('|');
    }

    if (app.isRunning()) {
        if (!message.isEmpty()) {
            app.sendMessage(message);
        }
//        return 0; // incomment later
    }

    MainWindow * mainWindow = new MainWindow();
    QObject::connect(&app, SIGNAL(messageAvailable(QString)), mainWindow, SLOT(receiveMessage(QString)));
    if (!message.isEmpty()) {
        mainWindow -> receiveMessage(message);
    }
    mainWindow -> show();

    return app.exec();
}
