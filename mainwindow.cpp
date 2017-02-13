#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QFile>
#include <QXmlStreamWriter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    grafScene = new GrafScene(ui->graph);

    grafAlgo = new GrafAlgorithm();
    grafAlgo->setTableAdjacency(ui->tableAdjacency);
    grafAlgo->setTableIncident(ui->tableIncidence);
    grafAlgo->setListResult(ui->listWidgetPath);

    connect(grafScene, &GrafScene::vertexAdded, this, &MainWindow::slotAddVertex);
    connect(grafScene, &GrafScene::vertexRemoved, this, &MainWindow::slotRemoveVertex);

    connect(grafScene, &GrafScene::vertexAdded, grafAlgo, &GrafAlgorithm::slotAddVertex);
    connect(grafScene, &GrafScene::vertexRemoved, grafAlgo, &GrafAlgorithm::slotRemoveVertex);
    connect(grafScene, &GrafScene::edgeAdded, grafAlgo, &GrafAlgorithm::slotAddEdge);
    connect(grafScene, &GrafScene::edgeRemoved, grafAlgo, &GrafAlgorithm::slotRemoveEdge);

    connect(grafAlgo, &GrafAlgorithm::clearLightingEdges, grafScene, &GrafScene::slotClearLightingEdges);
    connect(grafAlgo, &GrafAlgorithm::lightingEdge, grafScene, &GrafScene::slotLightingEdge);

    connect(this, &MainWindow::clickedButton, grafAlgo, &GrafAlgorithm::BFS);
    connect(this, &MainWindow::showPath, grafAlgo, &GrafAlgorithm::slotShowPath);
}

MainWindow::~MainWindow()
{
    delete grafScene;
//    delete grafAlgo;
    delete ui;
}

void MainWindow::slotAddVertex()
{
    count++;
    ui->spinBox->setMaximum(count);
    if(count == 1)
        ui->spinBox->setValue(1);
}

void MainWindow::slotRemoveVertex()
{
    count--;
    ui->spinBox->setMaximum(count);
}

void MainWindow::on_pushButtonExecute_clicked()
{
    if(ui->spinBox->value() == 0)
        return;

    emit clickedButton(ui->spinBox->value());
}

void MainWindow::on_listWidgetPath_doubleClicked(const QModelIndex &index)
{
    emit showPath(index.row());
}

bool MainWindow::setFilename()
{
    QFileDialog *dialog = new QFileDialog(this);
    dialog->setFileMode(QFileDialog::ExistingFile);
    dialog->setAcceptMode(QFileDialog::AcceptSave);
    dialog->setLabelText(QFileDialog::FileName, tr("Файл"));
    dialog->setLabelText(QFileDialog::FileType, tr("Тип"));
    dialog->setLabelText(QFileDialog::LookIn, tr("Виберите файл"));
    dialog->setLabelText(QFileDialog::Accept, tr("Выбрать"));
    dialog->setLabelText(QFileDialog::Reject, tr("Отмена"));

    if(dialog->exec())
    {
        filename = dialog->selectedFiles().first();
        return true;
    }
    else
        return false;
}

void MainWindow::on_actionCreate_triggered()
{

}

void MainWindow::on_actionSave_triggered()
{
    setFilename();
    save();
}

bool MainWindow::save()
{
    if(!filename.isEmpty())
    {
        QFile *file = new QFile(filename);
        if(!file->open(QIODevice::WriteOnly | QIODevice::Truncate))
            return false;

        QXmlStreamWriter xmlWriter(file);
        xmlWriter.setAutoFormatting(true);

        xmlWriter.writeStartDocument();
        xmlWriter.writeStartElement("root");

        xmlWriter.writeStartElement("vertices");
        for(int i = 0; i < grafScene->getCountVertices(); i++)
        {
            xmlWriter.writeStartElement("vertex");
            xmlWriter.writeAttribute("numVertex", QString::number(i + 1));
            xmlWriter.writeAttribute("x", QString::number(grafScene->getVertexPosX(i)));
            xmlWriter.writeAttribute("y", QString::number(grafScene->getVertexPosY(i)));
            xmlWriter.writeEndElement();
        }
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("edges");
        for(int i = 0; i < grafScene->getCountEdges(); i++)
        {
            xmlWriter.writeStartElement("edge");
            xmlWriter.writeAttribute("numEdge", QString::number(i + 1));
            xmlWriter.writeAttribute("vertexLeft", QString::number(grafScene->getEdgeVertexLeft(i)));
            xmlWriter.writeAttribute("vertexRight", QString::number(grafScene->getEdgeVertexRight(i)));
            xmlWriter.writeEndElement();
        }
        xmlWriter.writeEndElement();

        xmlWriter.writeEndElement();
        xmlWriter.writeEndDocument();
        file->close();
        return true;
    }
    else
        return false;
}

void MainWindow::on_actionExit_triggered()
{

}

void MainWindow::on_actionDownload_triggered()
{

}

void MainWindow::on_actionClose_triggered()
{

}

void MainWindow::on_actionSaveResult_triggered()
{

}

void MainWindow::on_actionClear_triggered()
{
    grafScene->clearScene();
}
