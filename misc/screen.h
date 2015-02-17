#ifndef SCREEN_H
#define SCREEN_H

//#include <QDesktopServices>
#include <QDesktopWidget>
#include <QApplication>

class Screen {
public:
    static void screenSize(int & width, int & height) {
        QDesktopWidget *desktop = QApplication::desktop();
        width = desktop -> width();
        height = desktop -> height();
    }

    static void minimizeIfNeeded(int & width, int & height, float percentage) {
        int screen_width, screen_height;
        percentage /= 100;
        screenSize(screen_width, screen_height);

        width = qMin((int)(screen_width * percentage), width);
        height = qMin((int)(screen_height * percentage), height);
    }
};

#endif // SCREEN_H
