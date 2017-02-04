#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

#include "GrafScene.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;

    GrafScene *grafScene;
};

#endif // WIDGET_H
