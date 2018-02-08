#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QMainWindow>

namespace Ui {
class HomeWindow;
}

class HomeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit HomeWindow(QWidget *parent = 0);
    ~HomeWindow();

private:
    Ui::HomeWindow *ui;
};

#endif // HOMEWINDOW_H
