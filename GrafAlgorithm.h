#ifndef GRAFALGORITHM_H
#define GRAFALGORITHM_H

#include <QObject>
#include <QWidget>
#include <QTableWidget>
#include <QListWidget>
#include <QVector>

class GrafAlgorithm : public QObject
{
    Q_OBJECT
public:
    explicit GrafAlgorithm(QObject *parent = 0);
    ~GrafAlgorithm();

    void setTableIncident(QTableWidget *value);
    void setTableAdjacency(QTableWidget *value);
    void setListResult(QListWidget *value);


//    void removeEdge();
//    void swapEdge();

signals:
    void clearLightingEdges();
    void lightingEdge(int vertexLeft, int vertexRight);

public slots:
    void slotAddVertex();
    void slotRemoveVertex(int index);

    void slotAddEdge(int vertexLeft, int vertexRight, bool directed);
    void slotRemoveEdge(int index, int vertexLeft, int vertexRight, bool directed);

    void BFS(int index);
    void slotShowPath(int index);

private:
    QTableWidget *tableIncident;            // Виджет матрицы инцидентности. Для выполнений операций с данной матрицой.
    QTableWidget *tableAdjacency;           // Виджет матрицы смежности
    QListWidget *listResult;                // Лист вывода результатабьль

    QVector<QVector<long long> > arrayAdjacency;
    QVector<QVector<int> > arrayIncident;
    QVector<QVector<int> > paths;

    int countVertices = 0;
    int countEdges = 0;
};

#endif // GRAFALGORITHM_H
