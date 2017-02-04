#include "widget.h"
#include "ui_widget.h"
#include "GrafScene.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    grafScene = new GrafScene(ui->graph);
}

Widget::~Widget()
{
    delete grafScene;
    delete ui;
}
