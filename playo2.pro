#-------------------------------------------------
#
# Project created by QtCreator 2014-01-17T01:55:48
#
#-------------------------------------------------

QT       += core gui webkitwidgets network
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
    misc/data_store.cpp \
    misc/icon_provider.cpp \
    misc/hotkey_manager.cpp \
    misc/settings.cpp \
    misc/format.cpp \
    web/ip_checker.cpp \
    web/web_api.cpp \
    web/socials/vk_api.cpp \
    web/socials/soundcloud_api.cpp \
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
    model/file_item.cpp \
    model/level_tree/level_tree_model.cpp \
    model/level_tree/level_tree_view.cpp \
    model/level_tree_breadcrumb/level_tree_breadcrumb_model.cpp \
    model/level_tree_breadcrumb/level_tree_breadcrumb_view.cpp \
    model/tree/tree_model.cpp \
    model/tree/tree_view.cpp \
    model/list/list_model.cpp \
    model/list/list_view.cpp \
    model/web/vk/vk_model.cpp \
    model/web/vk/vk_view.cpp \
    model/web/vk/vk_folder.cpp \
    model/web/vk/vk_file.cpp \
    model/web/soundcloud/soundcloud_model.cpp \
    model/web/soundcloud/soundcloud_view.cpp \
    model/web/soundcloud/soundcloud_playlist.cpp \
    model/web/soundcloud/soundcloud_file.cpp \
    model/model_item_delegate.cpp \
    model/model_item_state.cpp \
    model/hotkey_model/hotkey_delegate.cpp \
    model/hotkey_model/hotkey_editor.cpp \
    media/audio_player.cpp \
    media/player.cpp \
    media/mediainfo.cpp \
    media/library.cpp \
    media/genre.cpp \
    media/duration.cpp \
    override/toolbar.cpp \
    override/toolbarbutton.cpp \
    override/notify_timer.cpp \
    override/clickable_label.cpp \
    override/tabber.cpp \
    override/tab.cpp \
    override/custom_network_access_manager.cpp \
    override/slider.cpp \
    single_application.cpp \
    web/download.cpp \
    dialogs/relationdialog.cpp \
    override/spectrum.cpp \
    toolbars.cpp \
    web/auth_chemas/teu_auth.cpp \
    web/socials/vk_api_private.cpp \
    web/api_process.cpp \
    dialogs/captchadialog.cpp \
    model/web/deleted_list.cpp \
    model/web/web_model.cpp \
    misc/rand.cpp \
    misc/extensions.cpp \
    dialogs/extension_dialog.cpp \
    override/simple_list_view.cpp

HEADERS  += mainwindow.h \
    misc/data_store.h \
    misc/icon_provider.h \
    misc/hotkey_manager.h \
    misc/hotkey_types.h \
    misc/settings.h \
    misc/format.h \
    web/ip_checker.h\
    web/web_api.h\
    web/socials/vk_api.h\
    web/socials/soundcloud_api.h \
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
    model/file_item.h \
    model/level_tree/level_tree_model.h \
    model/level_tree/level_tree_view.h \
    model/level_tree_breadcrumb/level_tree_breadcrumb_model.h \
    model/level_tree_breadcrumb/level_tree_breadcrumb_view.h \
    model/tree/tree_model.h \
    model/tree/tree_view.h \
    model/list/list_model.h \
    model/list/list_view.h \
    model/web/vk/vk_model.h \
    model/web/vk/vk_view.h \
    model/web/vk/vk_folder.h \
    model/web/vk/vk_file.h \
    model/web/soundcloud/soundcloud_model.h \
    model/web/soundcloud/soundcloud_view.h \
    model/web/soundcloud/soundcloud_playlist.h \
    model/web/soundcloud/soundcloud_file.h \
    model/model_item_delegate.h \
    model/model_item_state.h \
    model/hotkey_model/hotkey_delegate.h \
    model/hotkey_model/hotkey_editor.h \
    media/audio_player.h \
    media/player.h \
    media/mediainfo.h \
    media/library.h \
    media/genre.h \
    media/duration.h \
    override/toolbar.h \
    override/toolbarbutton.h \
    override/slider_style.h \
    override/notify_timer.h \
    override/clickable_label.h\
    override/tabber.h \
    override/tab.h \
    override/custom_network_access_manager.h \
    override/slider.h \
    single_application.h \
    misc/func_container.h \
    web/download.h \
    dialogs/relationdialog.h \
    override/spectrum.h \
    toolbars.h \
    web/auth_chemas/teu_auth.h \
    web/socials/vk_api_private.h \
    web/api_process.h \
    dialogs/captchadialog.h \
    model/web/deleted_list.h \
    override/tree_view_style.h \
    model/web/web_model.h \
    misc/stylesheets.h \
    misc/rand.h \
    misc/extensions.h \
    dialogs/extension_dialog.h \
    override/simple_list_view.h
    model/view_types.h

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
    dialogs/settings_dialog.ui \
    dialogs/relationdialog.ui \
    dialogs/captchadialog.ui \
    dialogs/extension_dialog.ui

RESOURCES += \
    picts.qrc
