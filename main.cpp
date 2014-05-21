#include "mainwindow.h"
#include "single_application.h"
#include <qDebug>

int main(int argc, char *argv[]) {
    qRegisterMetaType<QVector <int> >("QVector<int>");

    QCoreApplication::setOrganizationName("BigBug");
    QCoreApplication::setOrganizationDomain("bigbug.sos");
    QCoreApplication::setApplicationName("Playo");

//    QCoreApplication::addLibraryPath("platforms");

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



////    QCheckBox {
////         spacing: 5px;
////     }

////     QCheckBox::indicator {
////         width: 13px;
////         height: 13px;
////     }

////     QCheckBox::indicator:unchecked {
////         image: url(:/images/checkbox_unchecked.png);
////     }

////     QCheckBox::indicator:unchecked:hover {
////         image: url(:/images/checkbox_unchecked_hover.png);
////     }

////     QCheckBox::indicator:unchecked:pressed {
////         image: url(:/images/checkbox_unchecked_pressed.png);
////     }

////     QCheckBox::indicator:checked {
////         image: url(:/images/checkbox_checked.png);
////     }

////     QCheckBox::indicator:checked:hover {
////         image: url(:/images/checkbox_checked_hover.png);
////     }

////     QCheckBox::indicator:checked:pressed {
////         image: url(:/images/checkbox_checked_pressed.png);
////     }

////     QCheckBox::indicator:indeterminate:hover {
////         image: url(:/images/checkbox_indeterminate_hover.png);
////     }

////     QCheckBox::indicator:indeterminate:pressed {
////         image: url(:/images/checkbox_indeterminate_pressed.png);
////     }


//    app.setStyleSheet("QCheckBox::indicator:checked  {"
//                                    "image: url(:/icon);"
//                                "});");

    mainWindow -> show();

    return app.exec();
}
