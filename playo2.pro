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
INCLUDEPATH += $$quote($${_PRO_FILE_PWD_}/libs/ffmpeg/include)
INCLUDEPATH += $$quote($${_PRO_FILE_PWD_}/libs/portable_audio/include)

#INCLUDEPATH += -L"$$_PRO_FILE_PWD_/libs/include"
#LIBS += -L"$$_PRO_FILE_PWD_/libs/" -lpsapi

#CONFIG += static thread windows x11 sse sse2 x86
CONFIG += static
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
    override/simple_list_view.cpp \
    media/media_player/media_player.cpp \
    media/media_player/stream_decoder.cpp \
    media/media_player/streams/stream.cpp \
    media/media_player/streams/audio_stream.cpp \
    media/media_player/streams/video_stream.cpp \
    media/media_player/streams/subtitle_stream.cpp \
    media/media_player/streams/audio_output_stream.cpp \
    media/media_player/streams/media_stream.cpp \
    media/libAV/utils/GPUMemCopy.cpp \
    media/libAV/utils/OpenGLHelper.cpp \
    media/libAV/AudioDecoder.cpp \
    media/libAV/AudioFormat.cpp \
    media/libAV/AudioFrame.cpp \
    media/libAV/AudioOutput.cpp \
    media/libAV/AudioOutputDSound.cpp \
    media/libAV/AudioOutputOpenAL.cpp \
    media/libAV/AudioOutputOpenSL.cpp \
    media/libAV/AudioOutputPortAudio.cpp \
    media/libAV/AudioOutputTypes.cpp \
    media/libAV/AudioResampler.cpp \
    media/libAV/AudioResamplerFF.cpp \
    media/libAV/AudioResamplerLibav.cpp \
    media/libAV/AudioResamplerTypes.cpp \
    media/libAV/AudioThread.cpp \
    media/libAV/AVClock.cpp \
    media/libAV/AVCompat.cpp \
    media/libAV/AVDecoder.cpp \
    media/libAV/AVDemuxer.cpp \
    media/libAV/AVDemuxThread.cpp \
    media/libAV/AVError.cpp \
    media/libAV/AVOutput.cpp \
    media/libAV/AVPlayer.cpp \
    media/libAV/AVThread.cpp \
    media/libAV/ColorTransform.cpp \
    media/libAV/CommonTypes.cpp \
    media/libAV/Direct2DRenderer.cpp \
    media/libAV/Filter.cpp \
    media/libAV/FilterContext.cpp \
    media/libAV/FilterManager.cpp \
    media/libAV/Frame.cpp \
    media/libAV/GDIRenderer.cpp \
    media/libAV/GLWidgetRenderer.cpp \
    media/libAV/GLWidgetRenderer2.cpp \
    media/libAV/GraphicsItemRenderer.cpp \
    media/libAV/ImageConverter.cpp \
    media/libAV/ImageConverterFF.cpp \
    media/libAV/ImageConverterIPP.cpp \
    media/libAV/LibAVFilter.cpp \
    media/libAV/OpenGLVideo.cpp \
    media/libAV/OutputSet.cpp \
    media/libAV/Packet.cpp \
    media/libAV/QAVIOContext.cpp \
    media/libAV/QPainterRenderer.cpp \
    media/libAV/QtAV_Global.cpp \
    media/libAV/ShaderManager.cpp \
    media/libAV/Statistics.cpp \
    media/libAV/VideoCapture.cpp \
    media/libAV/VideoDecoder.cpp \
    media/libAV/VideoDecoderCedarv.cpp \
    media/libAV/VideoDecoderCUDA.cpp \
    media/libAV/VideoDecoderDXVA.cpp \
    media/libAV/VideoDecoderFFmpeg.cpp \
    media/libAV/VideoDecoderFFmpegHW.cpp \
    media/libAV/VideoDecoderTypes.cpp \
    media/libAV/VideoDecoderVAAPI.cpp \
    media/libAV/VideoDecoderVDA.cpp \
    media/libAV/VideoFormat.cpp \
    media/libAV/VideoFrame.cpp \
    media/libAV/VideoOutput.cpp \
    media/libAV/VideoOutputEventFilter.cpp \
    media/libAV/VideoRenderer.cpp \
    media/libAV/VideoRendererTypes.cpp \
    media/libAV/VideoShader.cpp \
    media/libAV/VideoThread.cpp \
    media/libAV/WidgetRenderer.cpp \
    media/libAV/XVRenderer.cpp

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
    override/simple_list_view.h \
    media/media_player/media_player.h \
    media/media_player/media_player_libs.h \
    media/media_player/master_clock.h \
    media/media_player/media_player_utils.h \
    media/media_player/stream_decoder.h \
    media/media_player/streams/stream.h \
    media/media_player/streams/audio_stream.h \
    media/media_player/streams/video_stream.h \
    media/media_player/streams/subtitle_stream.h \
    media/media_player/streams/audio_output_stream.h \
    media/media_player/streams/media_stream.h \
    media/libAV/compat/msvc/inttypes.h \
    media/libAV/compat/msvc/stdint.h \
    media/libAV/private/AudioOutput_p.h \
    media/libAV/private/AudioResampler_p.h \
    media/libAV/private/AVCompat.h \
    media/libAV/private/AVDecoder_p.h \
    media/libAV/private/AVOutput_p.h \
    media/libAV/private/AVThread_p.h \
    media/libAV/private/factory.h \
    media/libAV/private/Filter_p.h \
    media/libAV/private/FilterManager.h \
    media/libAV/private/Frame_p.h \
    media/libAV/private/ImageConverter_p.h \
    media/libAV/private/OutputSet.h \
    media/libAV/private/QAVIOContext.h \
    media/libAV/private/QPainterRenderer_p.h \
    media/libAV/private/ShaderManager.h \
    media/libAV/private/singleton.h \
    media/libAV/private/VideoDecoder_p.h \
    media/libAV/private/VideoDecoderFFmpegHW_p.h \
    media/libAV/private/VideoOutputEventFilter.h \
    media/libAV/private/VideoRenderer_p.h \
    media/libAV/private/VideoShader_p.h \
    media/libAV/utils/BlockingQueue.h \
    media/libAV/utils/GPUMemCopy.h \
    media/libAV/utils/OpenGLHelper.h \
    media/libAV/AudioDecoder.h \
    media/libAV/AudioFormat.h \
    media/libAV/AudioFrame.h \
    media/libAV/AudioOutput.h \
    media/libAV/AudioOutputTypes.h \
    media/libAV/AudioResampler.h \
    media/libAV/AudioResamplerTypes.h \
    media/libAV/AudioThread.h \
    media/libAV/AVClock.h \
    media/libAV/AVDecoder.h \
    media/libAV/AVDemuxer.h \
    media/libAV/AVDemuxThread.h \
    media/libAV/AVError.h \
    media/libAV/AVOutput.h \
    media/libAV/AVPlayer.h \
    media/libAV/AVThread.h \
    media/libAV/ColorTransform.h \
    media/libAV/CommonTypes.h \
    media/libAV/dptr.h \
    media/libAV/FactoryDefine.h \
    media/libAV/Filter.h \
    media/libAV/FilterContext.h \
    media/libAV/Frame.h \
    media/libAV/GLWidgetRenderer.h \
    media/libAV/GLWidgetRenderer2.h \
    media/libAV/GraphicsItemRenderer.h \
    media/libAV/ImageConverter.h \
    media/libAV/ImageConverterTypes.h \
    media/libAV/LibAVFilter.h \
    media/libAV/OpenGLVideo.h \
    media/libAV/Packet.h \
    media/libAV/prepost.h \
    media/libAV/QPainterRenderer.h \
    media/libAV/QtAV.h \
    media/libAV/QtAV_Global.h \
    media/libAV/Statistics.h \
    media/libAV/SurfaceInterop.h \
    media/libAV/version.h \
    media/libAV/VideoCapture.h \
    media/libAV/VideoDecoder.h \
    media/libAV/VideoDecoderFFmpegHW.h \
    media/libAV/VideoDecoderTypes.h \
    media/libAV/VideoFormat.h \
    media/libAV/VideoFrame.h \
    media/libAV/VideoOutput.h \
    media/libAV/VideoRenderer.h \
    media/libAV/VideoRendererTypes.h \
    media/libAV/VideoShader.h \
    media/libAV/VideoThread.h \
    media/libAV/WidgetRenderer.h
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

        LIBS += -L"$$_PRO_FILE_PWD_/libs/ffmpeg/" -lswscale-2 -lavcodec -lavcodec-55 -lavdevice -lavdevice-55 -lavfilter -lavfilter-4 -lavformat -lavformat-55 -lavutil -lavutil-52 -lswresample -lswscale
        LIBS += -L"$$_PRO_FILE_PWD_/libs/ffmpeg/" -llibavcodec -llibavdevice -llibavfilter -llibavformat -llibavutil -llibpostproc -llibswresample -llibswscale -lpostproc -lpostproc-52 -lswresample-0

        LIBS += -L"$$_PRO_FILE_PWD_/libs/portable_audio/" -lportaudio_x86
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
