#ifndef GRAFSCENE_H
#define GRAFSCENE_H

#include <QObject>
#include <QQuickView>
#include <QWidget>
#include <QLayout>
#include <QQmlEngine>
#include <QQmlContext>

class GrafScene : public QObject
{
    Q_OBJECT
public:
    explicit GrafScene(QWidget *widget, QObject *parent = 0);
    ~GrafScene();

signals:
//    void lightingEdge(vertexLeft, vertexRight);

public slots:
    int addVertex();
    void removeVertex(int index);

private:
    QQuickView *quickView;
    QMap<int, QMap<int, QObject *> > edges;

    int count = 0;
};

#endif // GRAFSCENE_H
