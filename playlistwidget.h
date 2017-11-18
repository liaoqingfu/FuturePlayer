#ifndef PLAYLISTWIDGET_H
#define PLAYLISTWIDGET_H

#include <QWidget>
#include<QListWidget>
#include<QMouseEvent>
#include<QEvent>
#include<QScrollBar>
#include<QPoint>
#include<QString>
#include<QContextMenuEvent>

class PlayListWidget : public QListWidget
{
    Q_OBJECT
public:
    PlayListWidget(QWidget * parent = 0) ;
    void region(const QPoint &cursorGlobalPoint);
    void mouseDoubleClickEvent(QMouseEvent *);

    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void contextMenuEvent(QContextMenuEvent *menuEvent);
    void setIndex(int n){indexInPlayList = n;}
    int getIndex(){return indexInPlayList;}
signals:
    void ItemDoubleClick(QString name);
    void ItemDeleted(int n);
    void signalClearAll();
    void signalActionOpen();
    void signalClearValidItem();
    void signalOpenDirectory(int n);

public slots:
    void slotActionOpen();
    void slotActionDeleteItem();
    void slotActionClearValidItem();
    void slotActionClearAll();
    void slotActionPlay();
    void slotActionOpenDirectory();

private:
    int indexInPlayList;
    QPoint mousePos;
};

#endif // PLAYLISTWIDGET_H
