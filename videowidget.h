#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H



#include <QWidget>
#include"basewidget.h"
class VideoWidget : public BaseWidget
{
public:
    VideoWidget(QWidget*w=0);
    void setImage(QImage img);
public slots:
    void slot_imageMV(QImage);
protected:
    virtual void paintEvent(QPaintEvent*);
private:
    QImage m_img;
};
#endif // VIDEOWIDGET_H
