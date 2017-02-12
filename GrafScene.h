#ifndef GRAFSCENE_H
#define GRAFSCENE_H

#include <QObject>
#include <QQuickView>
#include <QWidget>
#include <QLayout>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickWidget>
#include <QVector>

class GrafScene : public QObject
{
    Q_OBJECT
public:
    explicit GrafScene(QWidget *widget, QObject *parent = 0);
    ~GrafScene();

    int getCountVertices();
    qreal getVertexPosX(int index);
    qreal getVertexPosY(int index);

    int getCountEdges();

//    void clearScene();

signals:
//    void lightingEdge(vertexLeft, vertexRight);
    void vertexAdded();
    void vertexRemoved(int index);

    void edgeAdded(int vertexLeft, int vertexRight, bool directed);
    void edgeRemoved(int index, int vertexLeft, int vertexRigt, bool directed);

public slots:
    void slotSetRoot(QObject* object);

    void slotAddVertex(QObject *object);
    void slotRemoveVertex(int index);

    void slotAddEdge(QObject *object, int vertexLeft, int vertexRight, bool directed = false);
    void slotRemoveEdge(int index, int vertexLeft, int vertexRight, bool directed = false);

    void slotClearLightingEdges();
    void slotLightingEdge(int indexVertexLeft, int indexVertexRight);


private:
    QQuickView *quickView;
    QVector<QObject *> vertices;
    QVector<QVector<QObject *> > edges;
    QObject *root;

    int countVertices = 0;
    int countEdges = 0;
};

#endif // GRAFSCENE_H
