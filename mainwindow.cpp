#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QFile>
#include <QXmlStreamWriter>
#include <QMessageBox>

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
    connect(grafAlgo, &GrafAlgorithm::showGraph, this, &MainWindow::slotShowGraph);

    connect(grafScene, &GrafScene::vertexAdded, grafAlgo, &GrafAlgorithm::slotAddVertex);
    connect(grafScene, &GrafScene::vertexRemoved, grafAlgo, &GrafAlgorithm::slotRemoveVertex);
    connect(grafScene, &GrafScene::edgeAdded, grafAlgo, &GrafAlgorithm::slotAddEdge);
    connect(grafScene, &GrafScene::edgeRemoved, grafAlgo, &GrafAlgorithm::slotRemoveEdge);

    connect(grafAlgo, &GrafAlgorithm::clearLightingVertices, grafScene, &GrafScene::slotClearLightingVertices);
    connect(grafAlgo, &GrafAlgorithm::lightingVertex, grafScene, &GrafScene::slotLightingVertex);
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

void MainWindow::slotShowGraph()
{
    ui->tabWidget->setCurrentIndex(1);
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

bool MainWindow::setFilenameSave()
{
    QFileDialog *dialog = new QFileDialog(this);
    dialog->setFileMode(QFileDialog::ExistingFile);
    dialog->setDirectory(QApplication::applicationDirPath());
    dialog->setNameFilter("Project file (*.project)");
    dialog->setAcceptMode(QFileDialog::AcceptSave);
    dialog->setLabelText(QFileDialog::FileName, tr("Файл"));
    dialog->setLabelText(QFileDialog::FileType, tr("Тип"));
    dialog->setLabelText(QFileDialog::LookIn, tr("Виберите файл"));
    dialog->setLabelText(QFileDialog::Accept, tr("Выбрать"));
    dialog->setLabelText(QFileDialog::Reject, tr("Отмена"));

    if(dialog->exec())
    {
        filename = dialog->selectedFiles().first();
        delete dialog;
        return true;
    }
    else
        return false;
}

void MainWindow::on_actionCreate_triggered()
{
    grafScene->clearScene();
    if(setFilenameSave())
    {
        ui->actionSave->setEnabled(true);
        ui->actionSaveAs->setEnabled(true);
        ui->actionClose->setEnabled(true);
        ui->actionSaveResult->setEnabled(true);
        ui->actionClear->setEnabled(true);
    }
}

void MainWindow::on_actionSave_triggered()
{
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

bool MainWindow::saveResult()
{
    // Выбор файла для сохранения результатов
    QString fileSaveName = QFileDialog::getSaveFileName(this, tr("Выберите файл"), QApplication::applicationDirPath(), "Text files (*.txt)");
    if(fileSaveName.isEmpty())
    {
        QMessageBox message;
        message.setIcon(QMessageBox::Critical);
        message.setWindowTitle(tr("Ошибка"));
        message.setText(tr("Выберите файл для сохранения!"));
        message.exec();
        return false;
    }

    QFile file(fileSaveName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        QMessageBox messageError;
        messageError.setIcon(QMessageBox::Critical);
        messageError.setWindowTitle(tr("Ошибка"));
        messageError.setText(tr("Не удалось открыть файл!"));
        messageError.exec();
        return false;
    }

    QTextStream data(&file);
    data.setCodec("UTF-8");
    // Матрица инцидентности
    data << QString(tr("Матрица инцидентности:\n"));
    data << QString("       ");
    for(int col = 0; col < ui->tableIncidence->columnCount() - 1; col++)
        data << QString("%1 ").arg(col + 1, 3);
    data << QString("%1").arg(ui->tableIncidence->columnCount(), 3);
    data << "\n";
    for(int row = 0; row < ui->tableIncidence->rowCount(); row++)
    {
        QString tmp;
        tmp += QString("%1 | ").arg(row + 1, 4);
        for(int col = 0; col < ui->tableIncidence->columnCount() - 1; col++)
            tmp += QString("%1 ").arg(ui->tableIncidence->item(row, col)->text().toInt(), 3);
        tmp += QString("%1 ").arg(ui->tableIncidence->item(row, ui->tableIncidence->columnCount() - 1)->text().toInt(), 3);
        data << tmp << endl;
    }
    // Матрица смежности
    data << QString(tr("\nМатрица смежности:\n"));
    data << QString("      ");
    for(int col = 0; col < ui->tableAdjacency->columnCount() - 1; col++)
        data << QString("%1 ").arg(col + 1, 3);
    data << QString("%1").arg(ui->tableAdjacency->columnCount(), 3);
    data << "\n";
    for(int row = 0; row < ui->tableAdjacency->rowCount(); row++)
    {
        QString tmp;
        tmp += QString("%1 | ").arg(row + 1, 3);
        for(int col = 0; col < ui->tableAdjacency->columnCount() - 1; col++)
            tmp += QString("%1 ").arg(ui->tableAdjacency->item(row, col)->text().toInt(), 3);

        tmp += QString("%1 ").arg(ui->tableAdjacency->item(row, ui->tableAdjacency->columnCount() - 1)->text().toInt(), 3);
        data << tmp << endl;
    }
    // Пути
    if(ui->listWidgetPath->count() != 0)
    {
        data << QString(tr("\nПоиск в ширину:\n"));
        for(int row = 0; row < ui->listWidgetPath->count(); row++)
            data << ui->listWidgetPath->item(row)->text() << endl;
    }
    file.close();
}

bool MainWindow::download()
{
    if(!filename.isEmpty())
    {
        QFile *file = new QFile(filename);
        if(!file->open(QIODevice::ReadOnly))
            return false;

        QXmlStreamReader xmlReader(file);

        xmlReader.readNext();

        while(!xmlReader.atEnd())
        {
            if(xmlReader.tokenType() != QXmlStreamReader::StartElement)
            {
                xmlReader.readNext();
                continue;
            }

            if(xmlReader.name() == "vertex" && xmlReader.tokenType() == QXmlStreamReader::StartElement)
                grafScene->addVertex(xmlReader.attributes().value("x").toDouble(), xmlReader.attributes().value("y").toDouble());

            if(xmlReader.name() == "edge" && xmlReader.tokenType() == QXmlStreamReader::StartElement)
                grafScene->addEdge(xmlReader.attributes().value("vertexLeft").toInt(), xmlReader.attributes().value("vertexRight").toInt());

            xmlReader.readNext();
        }

        return true;
    }
    else
        return false;
}

void MainWindow::on_actionExit_triggered()
{
    exit(0);
}

void MainWindow::on_actionDownload_triggered()
{
    grafScene->clearScene();
    setFilenameOpen();
    if(download())
    {
        ui->actionSave->setEnabled(true);
        ui->actionSaveAs->setEnabled(true);
        ui->actionClose->setEnabled(true);
        ui->actionSaveResult->setEnabled(true);
        ui->actionClear->setEnabled(true);
    }
}

void MainWindow::on_actionClose_triggered()
{
    grafScene->clearScene();
}

void MainWindow::on_actionSaveResult_triggered()
{
    saveResult();
}

void MainWindow::on_actionClear_triggered()
{
    ui->listWidgetPath->clear();
}

void MainWindow::on_actionSaveAs_triggered()
{
    setFilenameSave();
    save();
}

bool MainWindow::setFilenameOpen()
{
    QFileDialog *dialog = new QFileDialog(this);
    dialog->setFileMode(QFileDialog::ExistingFile);
    dialog->setDirectory(QApplication::applicationDirPath());
    dialog->setNameFilter("Project file (*.project)");
    dialog->setAcceptMode(QFileDialog::AcceptOpen);
    dialog->setLabelText(QFileDialog::FileName, tr("Файл"));
    dialog->setLabelText(QFileDialog::FileType, tr("Тип"));
    dialog->setLabelText(QFileDialog::LookIn, tr("Виберите файл"));
    dialog->setLabelText(QFileDialog::Accept, tr("Выбрать"));
    dialog->setLabelText(QFileDialog::Reject, tr("Отмена"));

    if(dialog->exec())
    {
        filename = dialog->selectedFiles().first();
        delete dialog;
        return true;
    }
    else
        return false;
}
