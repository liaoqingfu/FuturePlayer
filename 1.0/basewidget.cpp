#include "basewidget.h"
#include<QStyleOption>
#include<QPainter>
BaseWidget::BaseWidget(QWidget *parent) : QWidget(parent)
{
    setStyleSheet("QWidget{background:transparent;}");
    setMouseTracking(true);
}
void BaseWidget::paintEvent(QPaintEvent *e)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void BaseWidget::mousePressEvent(QMouseEvent *e)
{
    QWidget::mousePressEvent(e);
}

void BaseWidget::mouseMoveEvent(QMouseEvent *e)
{
   // if(mainWindow::GetInstance())
   //    mainWindow::GetInstance()->mouseMoveEvent(e);
    QWidget::mouseMoveEvent(e);
}

void BaseWidget::mouseReleaseEvent(QMouseEvent *e)
{
    QWidget::mouseReleaseEvent(e);
}
