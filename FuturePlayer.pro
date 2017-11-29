#-------------------------------------------------
#
# Project created by QtCreator 2017-11-18T17:19:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia multimediawidgets sql

TARGET = FuturePlayer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


INCLUDEPATH += $$PWD/ffmpeg/include
INCLUDEPATH += $$PWD/SDL2/include
LIBS += $$PWD/ffmpeg/lib/avformat.lib   \
        $$PWD/ffmpeg/lib/avcodec.lib    \
        $$PWD/ffmpeg/lib/avdevice.lib   \
        $$PWD/ffmpeg/lib/avfilter.lib   \
        $$PWD/ffmpeg/lib/avutil.lib     \
        $$PWD/ffmpeg/lib/postproc.lib   \
        $$PWD/ffmpeg/lib/swresample.lib \
        $$PWD/ffmpeg/lib/swscale.lib    \
        $$PWD/SDL2/lib/x86/SDL2.lib

SOURCES += \
        main.cpp \
        mainwidget.cpp \
    bottomwidget.cpp \
    playlistwidget.cpp \
    progressslider.cpp \
    videowidget.cpp \
    volumeslider.cpp \
    decoder.cpp \
    filter.cpp \
    videodisplay.cpp \
    audiooutput.cpp \
    player.cpp \
    urlprotocol.cpp \
    demuxer.cpp \
    avthread.cpp \
    audiothr.cpp \
    videothr.cpp \
    demuxerthr.cpp \
    ffdemux.cpp \
    formatcontext.cpp \
    buffer.cpp \
    ffcommon.cpp \
    writer.cpp

HEADERS += \
        mainwidget.h \
    bottomwidget.h \
    playlistwidget.h \
    progressslider.h \
    videowidget.h \
    volumeslider.h \
    decoder.h \
    filter.h \
    videodisplay.h \
    audiooutput.h \
    player.h \
    urlprotocol.h \
    demuxer.h \
    audiothr.h \
    videothr.h \
    demuxerthr.h \
    ffdemux.h \
    formatcontext.h \
    buffer.h \
    timestamp.h \
    packet.h \
    ffcommon.h \
    avthread.hpp \
    writer.hpp

FORMS += \
        mainwidget.ui

RESOURCES += \
    image/player/player.qrc
