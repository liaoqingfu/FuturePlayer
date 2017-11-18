#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include"VolumeSlider.h"
#include"ProgressSlider.h"
#include<QEvent>
#include<QMouseEvent>
#include <QWidget>
#include<QPoint>
#include<QUrl>
#include<QDragEnterEvent>
#include<QDropEvent>
#include<QListWidget>
#include<QPushButton>
#include<QVideoWidget>
#include<QContextMenuEvent>


class VideoWidget :public QVideoWidget
{
    Q_OBJECT
public:
    explicit VideoWidget(QWidget *parent = 0);
};

#endif // VIDEOWIDGET_H
