#include "GrafScene.h"

#include <QArgument>

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

int GrafScene::getCountVertices()
{
    return vertices.size();
}

qreal GrafScene::getVertexPosX(int index)
{
    return vertices[index]->property("x").toReal();
}

qreal GrafScene::getVertexPosY(int index)
{
    return vertices[index]->property("y").toReal();
}

int GrafScene::getCountEdges()
{
    return countEdges;
}

//void GrafScene::clearScene()
//{
//    QMetaObject::invokeMethod(root, "methodClearScene");
//}

void GrafScene::slotSetRoot(QObject *object)
{
    root = object;
}

void GrafScene::slotAddVertex(QObject *object)
{
    countVertices++;

    vertices.push_back(object);

    QVector<QObject *> tmp(countVertices, NULL);
    edges.push_back(tmp);

    for(int row = 0 ; row < edges.size() - 1; row++)
        edges[row].push_back(NULL);

    emit vertexAdded();
}

void GrafScene::slotRemoveVertex(int index)
{
    countVertices--;

    vertices.remove(index - 1);

    for(int row = 0 ; row < edges.size(); row++)
        edges[row].remove(index - 1);
    edges.remove(index - 1);

    emit vertexRemoved(index);
}

void GrafScene::slotAddEdge(QObject *object, int vertexLeft, int vertexRight, bool directed)
{
    countEdges++;

    edges[vertexLeft - 1][vertexRight - 1] = object;
    if(!directed)
        edges[vertexRight - 1][vertexLeft - 1] = object;

    emit edgeAdded(vertexLeft, vertexRight, directed);
}

void GrafScene::slotRemoveEdge(int index, int vertexLeft, int vertexRight, bool directed)
{
    countEdges--;

    edges[vertexLeft - 1][vertexRight - 1] = NULL;
    if(!directed)
        edges[vertexRight - 1][vertexLeft - 1] = NULL;

    emit edgeRemoved(index, vertexLeft, vertexRight, directed);
}

void GrafScene::slotClearLightingEdges()
{
    QMetaObject::invokeMethod(root, "methodClearLightingEdges");
}

void GrafScene::slotLightingEdge(int indexVertexLeft, int indexVertexRight)
{
    QMetaObject::invokeMethod(root, "methodAddLightingEdge", Q_ARG(QVariant, qVariantFromValue((QObject *) edges[indexVertexLeft][indexVertexRight])));
    QMetaObject::invokeMethod(edges[indexVertexLeft][indexVertexRight], "lightingEdge", Q_ARG(QVariant, true));
}
