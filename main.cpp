#include "mainwindow.h"
#include "single_application.h"
#include "misc/stylesheets.h"

//#include "memory_profiler/memory_profiler.h"

#include <QDebug>

int main(int argc, char *argv[]) {
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3, 2);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);


    qRegisterMetaType<QVector <int> >("QVector<int>");

//    qDebug() << "PATH " << QLibraryInfo::location(QLibraryInfo::PluginsPath);

    QCoreApplication::setOrganizationName("BigBug");
    QCoreApplication::setOrganizationDomain("bigbug.sos");
    QCoreApplication::setApplicationName("Playo");

//    QCoreApplication::addLibraryPath("platforms");

    SingleApplication app(argc, argv, "bigbugplayo");

//    app.setStyleSheet(QString("QWidget {background: black; color: gray;}"));
    app.setStyleSheet(Stylesheets::appStyles());

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
//        return 0; // uncomment later
    }

    int * i = new int[10];
    delete [] i;

    MainWindow * mainWindow = new MainWindow();
    QObject::connect(&app, SIGNAL(messageAvailable(QString)), mainWindow, SLOT(receiveMessage(QString)));
    if (!message.isEmpty()) {
        mainWindow -> receiveMessage(message);
    }

    mainWindow -> show();

    return app.exec();
}
