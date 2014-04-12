#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    qRegisterMetaType<QVector <int> >("QVector<int>");

    QCoreApplication::setOrganizationName("BigBug");
    QCoreApplication::setOrganizationDomain("bigbug.sos");
    QCoreApplication::setApplicationName("Playo");

    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}
