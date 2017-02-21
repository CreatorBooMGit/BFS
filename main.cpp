#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSplashScreen *splash = new QSplashScreen;
    QPixmap *pix = new QPixmap(":/images/graph.png");
    splash->setPixmap(*pix);
    splash->show();
    splash->showMessage(QObject::tr("Загрузка."), Qt::AlignLeft | Qt::AlignBottom, Qt::white);
    splash->showMessage(QObject::tr("Загрузка.."), Qt::AlignLeft | Qt::AlignBottom, Qt::white);
    splash->showMessage(QObject::tr("Загрузка..."), Qt::AlignLeft | Qt::AlignBottom, Qt::white);
    MainWindow w;
    splash->showMessage(QObject::tr("Запуск"), Qt::AlignLeft | Qt::AlignBottom, Qt::yellow);
    Sleep(5000);
    w.show();
    splash->finish(&w);
    delete splash;

    return a.exec();
}
