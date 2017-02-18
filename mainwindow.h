#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "GrafScene.h"
#include "GrafAlgorithm.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void clickedButton(int index);
    void showPath(int index);

private slots:
    void slotAddVertex();
    void slotRemoveVertex();
    void slotShowGraph();

    void on_pushButtonExecute_clicked();
    void on_listWidgetPath_doubleClicked(const QModelIndex &index);

    void on_actionCreate_triggered();
    void on_actionSave_triggered();
    void on_actionClose_triggered();
    void on_actionExit_triggered();
    void on_actionDownload_triggered();
    void on_actionSaveResult_triggered();
    void on_actionClear_triggered();

    void on_actionSaveAs_triggered();

    void on_listWidgetPath_customContextMenuRequested(const QPoint &pos);

    void on_actionHelp_triggered();

    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;

    GrafScene *grafScene;
    GrafAlgorithm *grafAlgo;

    int count = 0;

    bool createdProject = false;
    QString filename;

    bool setFilenameOpen();
    bool setFilenameSave();
    bool save();
    bool saveResult();
    bool download();
};

#endif // MAINWINDOW_H
