#include "GrafScene.h"

GrafScene::GrafScene(QWidget *widget, QObject *parent) : QObject(parent), quickView(new QQuickView)
{
    quickView->setResizeMode(QQuickView::SizeRootObjectToView);
    quickView->engine()->rootContext()->setContextProperty("graph", this);
    quickView->setSource(QUrl("qrc:/main.qml"));

    QWidget *container = QWidget::createWindowContainer(quickView);
    container->setMinimumSize(quickView->size());
    container->setFocusPolicy(Qt::TabFocus);

    widget->layout()->addWidget(container);
}

GrafScene::~GrafScene()
{

}

int GrafScene::addVertex()
{
    return ++count;
}

void GrafScene::removeVertex(int index)
{

}
