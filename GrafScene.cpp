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

int GrafScene::getEdgeVertexLeft(int index)
{
    return edges[index]->property("numVertexLeft").toInt();
}

int GrafScene::getEdgeVertexRight(int index)
{
    return edges[index]->property("numVertexRight").toInt();
}

void GrafScene::clearScene()
{
    QMetaObject::invokeMethod(root, "methodClearScene");
}

void GrafScene::addVertex(qreal posX, qreal posY)
{
    QMetaObject::invokeMethod(root, "methodAddVertex", Q_ARG(QVariant, posX), Q_ARG(QVariant, posY));
}

void GrafScene::addEdge(int numVertexLeft, int numVertexRight)
{
    QMetaObject::invokeMethod(root, "methodAddEdge", Q_ARG(QVariant, qVariantFromValue((QObject *) vertices[numVertexLeft - 1])),
            Q_ARG(QVariant, qVariantFromValue((QObject *) vertices[numVertexRight - 1])));
}

void GrafScene::slotSetRoot(QObject *object)
{
    root = object;
}

void GrafScene::slotAddVertex(QObject *object)
{
    countVertices++;

    vertices.push_back(object);

    QVector<QObject *> tmp(countVertices, NULL);
    matrixEdges.push_back(tmp);

    for(int row = 0 ; row < matrixEdges.size() - 1; row++)
        matrixEdges[row].push_back(NULL);

    emit vertexAdded();
}

void GrafScene::slotRemoveVertex(int index)
{
    countVertices--;

    vertices.remove(index - 1);

    for(int row = 0 ; row < matrixEdges.size(); row++)
        matrixEdges[row].remove(index - 1);
    matrixEdges.remove(index - 1);

    emit vertexRemoved(index);
}

void GrafScene::slotAddEdge(QObject *object, int vertexLeft, int vertexRight, bool directed)
{
    countEdges++;

    edges.push_back(object);

    matrixEdges[vertexLeft - 1][vertexRight - 1] = object;
    if(!directed)
        matrixEdges[vertexRight - 1][vertexLeft - 1] = object;

    emit edgeAdded(vertexLeft, vertexRight, directed);
}

void GrafScene::slotRemoveEdge(int index, int vertexLeft, int vertexRight, bool directed)
{
    countEdges--;

    edges.remove(index - 1);

    matrixEdges[vertexLeft - 1][vertexRight - 1] = NULL;
    if(!directed)
        matrixEdges[vertexRight - 1][vertexLeft - 1] = NULL;

    emit edgeRemoved(index, vertexLeft, vertexRight, directed);
}

void GrafScene::slotClearLightingVertices()
{
    QMetaObject::invokeMethod(root, "methodClearLightingVertices");
}

void GrafScene::slotLightingVertex(int index)
{
    QMetaObject::invokeMethod(root, "methodAddLightingVertex", Q_ARG(QVariant, qVariantFromValue((QObject *) vertices[index])));
    QMetaObject::invokeMethod(vertices[index], "lightingVertex", Q_ARG(QVariant, true));
}

void GrafScene::slotClearLightingEdges()
{
    QMetaObject::invokeMethod(root, "methodClearLightingEdges");
}

void GrafScene::slotLightingEdge(int indexVertexLeft, int indexVertexRight)
{
    QMetaObject::invokeMethod(root, "methodAddLightingEdge", Q_ARG(QVariant, qVariantFromValue((QObject *) matrixEdges[indexVertexLeft][indexVertexRight])));
    QMetaObject::invokeMethod(matrixEdges[indexVertexLeft][indexVertexRight], "lightingEdge", Q_ARG(QVariant, true));
}
