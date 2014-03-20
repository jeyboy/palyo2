#-------------------------------------------------
#
# Project created by QtCreator 2014-01-17T01:55:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT       += multimedia multimediawidgets

TARGET = playo2
TEMPLATE = app


DEFINES += TAGLIB_NO_CONFIG
#DEFINES += TAGLIB_STATIC=1

INCLUDEPATH += $$quote($${_PRO_FILE_PWD_}/libs/include)

#INCLUDEPATH += -L"$$_PRO_FILE_PWD_/libs/include"
#LIBS += -L"$$_PRO_FILE_PWD_/libs/" -lpsapi

CONFIG += static

SOURCES += main.cpp\
        mainwindow.cpp \
    tabber.cpp \
    tab.cpp \
    icon_provider.cpp \
    globalshortcut/qxtglobalshortcut.cpp \
    tabdialog.cpp \
    itemlist.cpp \
    model.cpp \
    model_item.cpp \
    library.cpp \
    model_item_delegate.cpp \
    data_store.cpp \
    player.cpp \
    toolbardialog.cpp \
    toolbarbuttondialog.cpp \
    toolbar.cpp \
    toolbarbutton.cpp \
    dnd.cpp \
    mediainfo.cpp \
    item_state.cpp

unix:!mac {
        QT += gui-private
        CONFIG += link_pkgconfig
        PKGCONFIG += x11
        LDFLAGS =- lX11
        DEFINES += HAVE_X11
        SOURCES += globalshortcut/qxtglobalshortcut_x11.cpp

        LIBS += $$quote($${_PRO_FILE_PWD_}/libs/taglib-project.a)
}
win32: {
        SOURCES += globalshortcut/qxtglobalshortcut_win.cpp
        LIBS += $$quote($${_PRO_FILE_PWD_}/libs/taglib-project.dll)
}
mac: {
        SOURCES += globalshortcut/qxtglobalshortcut_mac.cpp
}

HEADERS  += mainwindow.h \
    tabber.h \
    tab.h \
    icon_provider.h \
    globalshortcut/qxtglobal.h \
    globalshortcut/qxtglobalshortcut.h \
    globalshortcut/qxtglobalshortcut_p.h \
    tabdialog.h \
    itemlist.h \
    model.h \
    model_item.h \
    library.h \
    data_store.h \
    model_item_delegate.h \
    player.h \
    toolbardialog.h \
    toolbarbuttondialog.h \
    toolbar.h \
    toolbarbutton.h \
    dnd.h \
    mediainfo.h \
    item_state.h

FORMS    += mainwindow.ui \
    tabdialog.ui \
    toolbardialog.ui \
    toolbarbuttondialog.ui

RESOURCES += \
    picts.qrc
