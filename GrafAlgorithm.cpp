#include "GrafAlgorithm.h"

#include <QDebug>
#include <QQueue>
#include <iostream>

using namespace std;

GrafAlgorithm::GrafAlgorithm(QObject *parent) : QObject(parent)
{

}

GrafAlgorithm::~GrafAlgorithm()
{

}

void GrafAlgorithm::setTableIncident(QTableWidget *value)
{
    tableIncident = value;
}

void GrafAlgorithm::setTableAdjacency(QTableWidget *value)
{
    tableAdjacency = value;
}

void GrafAlgorithm::setListResult(QListWidget *value)
{
    listResult = value;
}

void GrafAlgorithm::slotAddVertex()
{
    countVertices++;

    QVector<int> tmp(countEdges, 0);
    arrayIncident.push_back(tmp);

//    tableIncident->insertColumn(arrayIncident.size() - 1);
    tableIncident->insertRow(arrayIncident.size() - 1);

    for(int col = 0 ; col < arrayIncident.size(); col++)
    {
        QTableWidgetItem *item = new QTableWidgetItem("0");
        item->setTextAlignment(Qt::AlignCenter);
        tableIncident->setItem(arrayIncident.size() - 1, col, item);
    }


    QVector<long long> tmpAd(countVertices, 0);
    arrayAdjacency.push_back(tmpAd);

    for(int col = 0 ; col < arrayAdjacency.size() - 1; col++)
        arrayAdjacency[col].push_back(0);

    tableAdjacency->insertColumn(arrayAdjacency.size() - 1);
    tableAdjacency->insertRow(arrayAdjacency.size() - 1);

    for(int col = 0 ; col < arrayAdjacency.size(); col++)
    {
        QTableWidgetItem *item = new QTableWidgetItem("∞");
        item->setTextAlignment(Qt::AlignCenter);
        tableAdjacency->setItem(arrayAdjacency.size() - 1, col, item);
    }

    for(int row = 0 ; row < arrayAdjacency.size(); row++)
    {
        QTableWidgetItem *item = new QTableWidgetItem("∞");
        item->setTextAlignment(Qt::AlignCenter);
        tableAdjacency->setItem(row, arrayAdjacency.size() - 1, item);
    }

    QTableWidgetItem *itemZero = new QTableWidgetItem("∞");
    itemZero->setTextAlignment(Qt::AlignCenter);
    tableAdjacency->setItem(arrayAdjacency.size() - 1, arrayAdjacency.size() - 1, itemZero);
}

void GrafAlgorithm::slotRemoveVertex(int index)
{
    index--;
    countVertices--;
    arrayAdjacency.remove(index);

    for(int col = 0; col < arrayAdjacency.size(); col++)
        arrayAdjacency[col].remove(index);

    tableAdjacency->removeColumn(index);
    tableAdjacency->removeRow(index);

    for(int col = 0; col < arrayIncident[index].size(); col++)
    {
        if(arrayIncident[index][col] != 0)
            for(int row = 0; row < arrayIncident.size(); row++)
            {
                arrayIncident[row].remove(col);
            }
    }
    arrayIncident.remove(index);

    tableIncident->removeRow(index);
}

void GrafAlgorithm::slotAddEdge(int vertexLeft, int vertexRight, bool directed)
{
    countEdges++;

    for(int row = 0; row < arrayIncident.size(); row++)
        arrayIncident[row].push_back(0);

    tableIncident->insertColumn(countEdges - 1);
    for(int row = 0 ; row < arrayIncident.size(); row++)
    {
        QTableWidgetItem *item = new QTableWidgetItem("0");
        item->setTextAlignment(Qt::AlignCenter);
        tableIncident->setItem(row, countEdges - 1, item);
    }

    if(!directed)
    {
        arrayIncident[vertexLeft - 1][countEdges - 1] = 1;
        QTableWidgetItem *itemVL = new QTableWidgetItem("1");
        itemVL->setTextAlignment(Qt::AlignCenter);
        tableIncident->setItem(vertexLeft - 1, countEdges - 1, itemVL);
    }
    else
    {
        arrayIncident[vertexLeft - 1][countEdges - 1] = -1;
        QTableWidgetItem *itemVL = new QTableWidgetItem("-1");
        itemVL->setTextAlignment(Qt::AlignCenter);
        tableIncident->setItem(vertexLeft - 1, countEdges - 1, itemVL);
    }

    arrayIncident[vertexRight - 1][countEdges - 1] = 1;
    QTableWidgetItem *itemVR = new QTableWidgetItem("1");
    itemVR->setTextAlignment(Qt::AlignCenter);
    tableIncident->setItem(vertexRight - 1, countEdges - 1, itemVR);


    arrayAdjacency[vertexLeft - 1][vertexRight - 1] = 1;
    QTableWidgetItem *itemTA = new QTableWidgetItem("1");
    itemTA->setTextAlignment(Qt::AlignCenter);
    tableAdjacency->setItem(vertexLeft - 1, vertexRight - 1, itemTA);
    if(!directed)
    {
        arrayAdjacency[vertexRight - 1][vertexLeft - 1] = 1;
        QTableWidgetItem *itemTA2 = new QTableWidgetItem("1");
        itemTA2->setTextAlignment(Qt::AlignCenter);
        tableAdjacency->setItem(vertexRight - 1, vertexLeft - 1, itemTA2);
    }
}

void GrafAlgorithm::slotRemoveEdge(int index, int vertexLeft, int vertexRight, bool directed)
{
    countEdges--;

    for(int row = 0; row < arrayIncident.size(); row++)
        arrayIncident[row].remove(index - 1);

    tableIncident->removeColumn(index - 1);

    arrayAdjacency[vertexLeft - 1][vertexRight - 1] = 0;
    tableAdjacency->item(vertexLeft - 1, vertexRight - 1)->setText("0");
    if(!directed)
    {
        arrayAdjacency[vertexRight - 1][vertexLeft - 1] = 0;
        tableAdjacency->item(vertexRight - 1, vertexLeft - 1)->setText("0");
    }
}

void GrafAlgorithm::BFS(int index)
{
    listResult->clear();
    paths.clear();

    index--;
    QQueue<int> q;
    q.enqueue(index);

    QVector<bool> used(countVertices, false);
    QVector<int> d(countVertices), p(countVertices);
    used[index] = true;
    p[index] = -1;
    while(!q.empty())
    {
        int v = q.dequeue();
        for(size_t i = 0; i < arrayAdjacency[v].size(); i++)
        {
            if(arrayAdjacency[v][i] != 0)
            {
                if(!used[i])
                {
                    used[i] = true;
                    q.enqueue(i);
                    d[i] = d[v] + 1;
                    p[i] = v;
                }
            }
        }
    }

    for(int vertex = 0; vertex < countVertices; vertex++)
    {
        if(!used[vertex])
        {
            QString str;
            str.append(tr("Вершина %1: %2").arg(vertex + 1).arg(tr("Путь не найден")));
            listResult->addItem(str);
//            qDebug() << str;
            continue;
        }
        if(vertex == index)
            continue;

        QVector<int> path;
        for (int v = vertex; v != -1; v = p[v])
            path.push_back(v);

        reverse(path.begin(), path.end());
        QString str;
        str.append(tr("Вершина %1:").arg(vertex + 1));
        for (size_t i = 0; i < path.size() - 1; i++)
            str.append(QString(" %1 ->").arg(path[i] + 1));
        str.append(QString(" %1").arg(path[path.size() - 1] + 1));
        paths.push_back(path);
        listResult->addItem(str);
//        qDebug() << str;
    }
}

void GrafAlgorithm::slotShowPath(int index)
{
    emit clearLightingEdges();
    for(int i = 0; i < paths[index].size() - 1; i++)
        emit lightingEdge(paths[index][i], paths[index][i + 1]);
}
