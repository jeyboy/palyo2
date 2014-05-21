#-------------------------------------------------
#
# Project created by QtCreator 2014-01-17T01:55:48
#
#-------------------------------------------------

QT       += core gui webkitwidgets
QT       += multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = playo2
TEMPLATE = app


DEFINES += TAGLIB_NO_CONFIG
#DEFINES += TAGLIB_STATIC=1

INCLUDEPATH += $$quote($${_PRO_FILE_PWD_}/libs/taglib/include)
INCLUDEPATH += $$quote($${_PRO_FILE_PWD_}/libs/bass/include)

#INCLUDEPATH += -L"$$_PRO_FILE_PWD_/libs/include"
#LIBS += -L"$$_PRO_FILE_PWD_/libs/" -lpsapi

#CONFIG += static thread windows x11 sse sse2 x86
CONFIG += static

SOURCES += main.cpp\
    mainwindow.cpp \
    data_store.cpp \
    misc/icon_provider.cpp \
    misc/file_size.cpp \
    misc/hotkey_manager.cpp \
    web/ip_checker.cpp \
    web/web_api.cpp \
    web/socials/vk_api.cpp \
    globalshortcut/qxtglobalshortcut.cpp \
    dialogs/tabdialog.cpp \
    dialogs/toolbardialog.cpp \
    dialogs/toolbarbuttondialog.cpp \
    dialogs/web_dialog.cpp \
    dialogs/settings_dialog.cpp \
    model/hotkey_model/hotkey_model.cpp \
    model/hotkey_model/hotkey_model_item.cpp \
    model/view.cpp \
    model/model.cpp \
    model/model_item.cpp \
    model/folder_item.cpp \
    model/web_folder_item.cpp \
    model/web_file_item.cpp \
    model/file_item.cpp \
    model/level_tree/level_tree_model.cpp \
    model/level_tree/level_tree_view.cpp \
    model/tree/tree_model.cpp \
    model/tree/tree_view.cpp \
    model/list/list_model.cpp \
    model/list/list_view.cpp \
    model/web/vk/vk_model.cpp \
    model/web/vk/vk_view.cpp \
    model/model_item_delegate.cpp \
    model/model_item_state.cpp \
    media/audio_player.cpp \
    media/player.cpp \
    media/mediainfo.cpp \
    media/library.cpp \
    media/library_item.cpp \
    override/toolbar.cpp \
    override/toolbarbutton.cpp \
    override/notify_timer.cpp \
    override/clickable_label.cpp \
    override/tabber.cpp \
    override/tab.cpp \
    override/custom_network_access_manager.cpp \
    single_application.cpp \
    misc/settings.cpp \
    model/hotkey_model/hotkey_delegate.cpp \
    model/hotkey_model/hotkey_editor.cpp \
    override/slider.cpp


HEADERS  += mainwindow.h \
    data_store.h \
    misc/icon_provider.h \
    misc/file_size.h \
    misc/hotkey_manager.h \
    web/ip_checker.h\
    web/web_api.h\
    web/socials/vk_api.h\
#    file_registration/file_registration.h \
    globalshortcut/qxtglobal.h \
    globalshortcut/qxtglobalshortcut.h \
    globalshortcut/qxtglobalshortcut_p.h \
    dialogs/tabdialog.h \
    dialogs/toolbardialog.h \
    dialogs/toolbarbuttondialog.h \
    dialogs/web_dialog.h \
    dialogs/settings_dialog.h \
    model/hotkey_model/hotkey_model.h \
    model/hotkey_model/hotkey_model_item.h \
    model/view.h \
    model/model.h \
    model/model_item.h \
    model/folder_item.h \
    model/web_folder_item.h \
    model/web_file_item.h \
    model/file_item.h \
    model/level_tree/level_tree_model.h \
    model/level_tree/level_tree_view.h \
    model/tree/tree_model.h \
    model/tree/tree_view.h \
    model/list/list_model.h \
    model/list/list_view.h \
    model/web/vk/vk_model.h \
    model/web/vk/vk_view.h \
    model/model_item_delegate.h \
    model/model_item_state.h \
    media/audio_player.h \
    media/player.h \
    media/mediainfo.h \
    media/library.h \
    media/library_item.h \
    override/toolbar.h \
    override/toolbarbutton.h \
    override/slider_style.h \
    override/notify_timer.h \
    override/clickable_label.h\
    override/tabber.h \
    override/tab.h \
    override/custom_network_access_manager.h \
    single_application.h \
    misc/settings.h \
    model/hotkey_model/hotkey_delegate.h \
    model/hotkey_model/hotkey_editor.h \
    misc/hotkey_types.h \
    override/slider.h

unix:!mac {
        QT += gui-private
        CONFIG += link_pkgconfig
        PKGCONFIG += x11
        LDFLAGS =- lX11
        DEFINES += HAVE_X11
        SOURCES += globalshortcut/qxtglobalshortcut_x11.cpp
#            file_registration/file_registration_x11.cpp

        LIBS += $$quote($${_PRO_FILE_PWD_}/libs/taglib/taglib-project.a)
        LIBS += $$quote($${_PRO_FILE_PWD_}/libs/bass/libbass.so)
}
win32: {
        SOURCES += globalshortcut/qxtglobalshortcut_win.cpp
#            file_registration/file_registration_win.cpp
        LIBS += $$quote($${_PRO_FILE_PWD_}/libs/taglib/taglib-project.dll)
        LIBS += $$quote($${_PRO_FILE_PWD_}/libs/bass/bass.lib)
        LIBS += $$quote($${_PRO_FILE_PWD_}/libs/bass/bass_fx.lib)
        LIBS += $$quote($${_PRO_FILE_PWD_}/libs/bass/bassmix.lib)
}
mac: {
        SOURCES += globalshortcut/qxtglobalshortcut_mac.cpp
#            file_registration/file_registration_mac.cpp
        LIBS += $$quote($${_PRO_FILE_PWD_}/libs/bass/libbass.dylib)
}

FORMS    += mainwindow.ui \
    dialogs/tabdialog.ui \
    dialogs/toolbardialog.ui \
    dialogs/toolbarbuttondialog.ui \
    dialogs/web_dialog.ui \
    dialogs/settings_dialog.ui

RESOURCES += \
    picts.qrc
