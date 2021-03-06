#include "mainwidget.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QDir>
#include "demuxerfactory.hpp"
#include "ffdemux.h"

void RegisterModule()
{

#ifndef QT_DEBUG
    av_log_set_level(AV_LOG_FATAL);
#endif
    av_register_all();
    avformat_network_init();

    REGISTER_DEMUXER(FFDemux);

}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::addLibraryPath("image/player");
    QString name = QDir::currentPath();
    QPixmap appIcon((name+"/appIcon.png"));

    RegisterModule();

    a.setWindowIcon(appIcon);

    MainWidget w;


    int width = QApplication::desktop()->availableGeometry().width();
    int height = QApplication::desktop()->availableGeometry().height();
    w.setGeometry((width-w.getWindowIniWidth())/2, (height-w.getWindowIniHeight())/2, w.getWindowIniWidth(), w.getWindowIniHeight());
    w.show();

    return a.exec();
}
