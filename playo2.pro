#-------------------------------------------------
#
# Project created by QtCreator 2014-01-17T01:55:48
#
#-------------------------------------------------

QT       += core gui webkitwidgets network opengl
QT       += multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = playo2
TEMPLATE = app
RC_FILE = playo.rc

DEFINES += TAGLIB_NO_CONFIG
#DEFINES += TAGLIB_STATIC=1

INCLUDEPATH += $$quote($${_PRO_FILE_PWD_}/libs/taglib/include)
INCLUDEPATH += $$quote($${_PRO_FILE_PWD_}/libs/bass/include)
INCLUDEPATH += $$quote($${_PRO_FILE_PWD_}/libs/ffmpeg/include)
INCLUDEPATH += $$quote($${_PRO_FILE_PWD_}/libs/portable_audio/include)

#INCLUDEPATH += -L"$$_PRO_FILE_PWD_/libs/include"
#LIBS += -L"$$_PRO_FILE_PWD_/libs/" -lpsapi

#CONFIG += static thread windows x11 sse sse2 x86
CONFIG += static #release
QMAKE_CXXFLAGS += -D__STDC_CONSTANT_MACROS

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
    media/media_player/utils/genre.cpp \
    media/media_player/utils/duration.cpp \
    override/toolbar.cpp \
    override/toolbarbutton.cpp \
    override/notify_timer.cpp \
    override/clickable_label.cpp \
    override/tabber.cpp \
    override/tab.cpp \
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
    override/simple_list_view.cpp \
    memory_profiler/memory_profiler.cpp \
    media/media_player/media_player.cpp \
    media/media_player/streams/stream_decoder.cpp \
    media/media_player/streams/base/stream.cpp \
    media/media_player/streams/base/media_stream.cpp \
    media/media_player/streams/decoders/audio_stream.cpp \
    media/media_player/streams/decoders/video_stream.cpp \
    media/media_player/streams/decoders/subtitle_stream.cpp \
    media/media_player/resamplers/audio_resampler.cpp \
    media/media_player/resamplers/video_resampler.cpp \
    media/media_player/utils/master_clock.cpp \
    media/media_player/output/audio/audio_output_stream.cpp \
    media/media_player/output/video/controls/control_panel.cpp \
    media/media_player/output/video/controls/output_container.cpp \
    media/media_player/output/video/video_output.cpp \
    media/media_player/output/video/controls/renders/render_interface.cpp \
    media/media_player/output/video/controls/renders/hardware_render.cpp \
    media/media_player/output/video/controls/renders/gl_render.cpp \
    media/media_player/output/video/controls/renders/gl_render_raw.cpp \
    media/media_player/utils/video_types.cpp \ 
    media/media_player/utils/web/custom_network_access_manager.cpp \
    media/media_player/utils/web/web_object.cpp \
    media/media_player/contexts/custom_context.cpp \
    media/media_player/output/video/controls/renders/stub_render.cpp

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
    globalshortcut/qxtglobal.h \
    globalshortcut/qxtglobalshortcut.h \
    globalshortcut/qxtglobalshortcut_p.h \
    dialogs/tabdialog.h \
    dialogs/toolbardialog.h \
    dialogs/toolbarbuttondialog.h \
    dialogs/web_dialog.h \
    dialogs/settings_dialog.h \
    model/view_types.h \
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
    media/media_player/utils/genre.h \
    media/media_player/utils/duration.h \
    override/toolbar.h \
    override/toolbarbutton.h \
    override/slider_style.h \
    override/notify_timer.h \
    override/clickable_label.h\
    override/tabber.h \
    override/tab.h \
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
    misc/screen.h \
    dialogs/extension_dialog.h \
    override/simple_list_view.h \
    memory_profiler/memory_profiler.h \
    media/media_player/media_player.h \
    media/media_player/utils/media_player_libs.h \
    media/media_player/utils/master_clock.h \
    media/media_player/utils/media_player_utils.h \
    media/media_player/streams/stream_decoder.h \
    media/media_player/streams/base/stream.h \
    media/media_player/streams/base/media_stream.h \
    media/media_player/streams/decoders/audio_stream.h \
    media/media_player/streams/decoders/video_stream.h \
    media/media_player/streams/decoders/subtitle_stream.h \
    media/media_player/resamplers/audio_resampler.h \
    media/media_player/resamplers/video_resampler.h \
    media/media_player/streams/base/i_media_stream.h \
    media/media_player/output/audio/audio_output_stream.h \
    media/media_player/output/video/controls/control_panel.h \
    media/media_player/output/video/controls/output_container.h \
    media/media_player/output/video/video_output.h \
    media/media_player/output/video/controls/renders/render_interface.h \
    media/media_player/output/video/controls/renders/hardware_render.h \
    media/media_player/output/video/controls/renders/gl_render.h \
    media/media_player/output/video/controls/renders/renders.h \
    media/media_player/output/video/controls/renders/gl_render_raw.h \
    media/media_player/streams/frames/video/color_conversion.h \
    media/media_player/streams/frames/video/shader.h \
    media/media_player/streams/frames/video/video_buffer.h \
    media/media_player/streams/frames/video/video_settings.h \
    media/media_player/streams/frames/audio_frame.h \
    media/media_player/streams/frames/video_frame.h \
    media/media_player/output/video/controls/renders/render_types.h \
    media/media_player/utils/video_types.h \
    media/media_player/streams/frames/video/video_blending.h \
    media/media_player/utils/system_utils.h \
    media/media_player/output/audio/rdft_spectrum.h \
    media/media_player/output/audio/fft_spectrum.h \
    media/media_player/utils/web/custom_network_access_manager.h \
    media/media_player/utils/web/web_object.h \
    media/media_player/contexts/custom_context.h \
    media/media_player/streams/frames/video/video_buffer_proxy.h \
    media/media_player/utils/media_attributes.h \
    media/media_player/media_player_state.h \
    media/media_player/output/video/controls/renders/stub_render.h \
    media/media_player/output/audio/audio_output_types.h \
    media/media_player/utils/audio/audio_device.h \
    media/media_player/utils/audio/audio_info.h \
    media/media_player/utils/audio/qt_audio_device.h \
    media/media_player/utils/audio/port_audio_device.h

unix:!mac {
        QT += gui-private
        CONFIG += link_pkgconfig
        PKGCONFIG += x11
        LDFLAGS =- lX11
        DEFINES += HAVE_X11
        SOURCES += globalshortcut/qxtglobalshortcut_x11.cpp

#        LIBS += $$quote($${_PRO_FILE_PWD_}/libs/bass/libbass.so)
}
win32: {
        LIBS += -lpsapi
        LIBS += $$quote($${_PRO_FILE_PWD_}/libs/bass/bass.lib)
        LIBS += $$quote($${_PRO_FILE_PWD_}/libs/bass/bass_fx.lib)
        LIBS += $$quote($${_PRO_FILE_PWD_}/libs/bass/bassmix.lib)

        LIBS += -L"$$_PRO_FILE_PWD_/libs/ffmpeg/" -lswscale-3 -lavcodec -lavcodec-56 -lavdevice -lavdevice-56 -lavfilter -lavfilter-5 -lavformat -lavformat-56 -lavutil -lavutil-54 -lswresample -lswscale
        LIBS += -L"$$_PRO_FILE_PWD_/libs/ffmpeg/" -llibavcodec -llibavdevice -llibavfilter -llibavformat -llibavutil -llibpostproc -llibswresample -llibswscale -lpostproc -lpostproc-53 -lswresample-1

        LIBS += -L"$$_PRO_FILE_PWD_/libs/portable_audio/" -lportaudio_x86

        HEADERS  += media/media_player/output/audio/portaudio_output_stream.h

        SOURCES += globalshortcut/qxtglobalshortcut_win.cpp  \
            media/media_player/output/audio/portaudio_output_stream.cpp
}
mac: {
        LIBS += $$quote($${_PRO_FILE_PWD_}/libs/bass/libbass.dylib)

        SOURCES += globalshortcut/qxtglobalshortcut_mac.cpp
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

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/taglib/ -ltaglib-project
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/taglib/ -ltaglib-project
else:unix:!macx: LIBS += -L$$PWD/libs/taglib/ -ltaglib-project

INCLUDEPATH += $$PWD/libs/taglib
DEPENDPATH += $$PWD/libs/taglib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/bass/ -lbass
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/bass/ -lbass
else:unix:!macx: LIBS += -L$$PWD/libs/bass/ -lbass

INCLUDEPATH += $$PWD/libs/bass
DEPENDPATH += $$PWD/libs/bass
