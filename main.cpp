#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSplashScreen *splash = new QSplashScreen;
    QPixmap *pix = new QPixmap(":/images/the_matrix.png");
//    pix->scaledToWidth(640);
    splash->setPixmap(*pix);
    splash->show();
    splash->showMessage(QObject::tr("Курсовая работа \"Алгоритм поискав ширину\""), Qt::AlignLeft | Qt::AlignTop, Qt::white);
    splash->showMessage(QObject::tr("Загрузка..."), Qt::AlignLeft | Qt::AlignBottom, Qt::white);
    MainWindow w;
    splash->showMessage(QObject::tr("Запуск"), Qt::AlignLeft | Qt::AlignBottom, Qt::yellow);
    w.show();
    splash->finish(&w);
    delete splash;

    return a.exec();
}
