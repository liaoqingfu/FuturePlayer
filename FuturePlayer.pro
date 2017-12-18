#-------------------------------------------------
#
# Project created by QtCreator 2017-11-18T17:19:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia multimediawidgets sql

TARGET = FuturePlayer
TEMPLATE = app
CONFIG   += c++11
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
    writer.cpp \
    audiofilter.cpp \
    sndresampler.cpp \
    videowriter.cpp \
    videofilters.cpp \
    playerosd.cpp \
    videoframe.cpp \
    ffdec.cpp \
    streaminfo.cpp \
    setting.cpp \
    modulefactory.cpp \
    packetbuffer.cpp \
    basewidget.cpp \
    demuxerfactory.cpp \
    openthr.cpp \
    functions.cpp \
    audiosdl2.cpp \
    framequeue.cpp \
    ringbuffer.cpp

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
    urlprotocol.h \
    demuxer.h \
    audiothr.h \
    ffdemux.h \
    formatcontext.h \
    timestamp.h \
    packet.h \
    ffcommon.h \
    avthread.hpp \
    writer.hpp \
    audiofilter.hpp \
    sndresampler.hpp \
    videothr.hpp \
    pixelformat.hpp \
    videowriter.hpp \
    videofilters.hpp \
    playerosd.hpp \
    videoframe.hpp \
    buffer.hpp \
    ffdec.hpp \
    streaminfo.hpp \
    setting.hpp \
    modulefactory.hpp \
    demuxerthr.hpp \
    packetbuffer.hpp \
    player.hpp \
    basewidget.h \
    demuxerfactory.hpp \
    openthr.hpp \
    functions.hpp \
    audiosdl2.hpp \
    framequeue.h \
    ringbuffer.hpp

FORMS += \
        mainwidget.ui

RESOURCES += \
    image/player/player.qrc
