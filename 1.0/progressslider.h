#ifndef PROGRESSSLIDER_H
#define PROGRESSSLIDER_H

#include<QSlider>
#include<QMouseEvent>
#include<QEvent>

class ProgressSlider : public QSlider
{
    Q_OBJECT
public:
    explicit ProgressSlider(Qt::Orientation orientation, QWidget* parent =0);
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent * event);
signals:
    void signalPressPosition(qint64 position);

public slots:

};

#endif // PROGRESSSLIDER_H
