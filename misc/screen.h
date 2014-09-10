#ifndef SCREEN_H
#define SCREEN_H

#include <QDesktopServices>
#include <QDesktopWidget>
#include <QApplication>

class Screen {
public:
    static void screenSize(int & width, int & height) {
        QDesktopWidget *desktop = QApplication::desktop();
        width = desktop -> width();
        height = desktop -> height();
    }
};

#endif // SCREEN_H
