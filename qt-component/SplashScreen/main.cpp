#include "mainwindow.h"

#include <QApplication>

#include "splashscreen.h"

#include "qthread.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SplashScreen splash;
    splash.show();
    a.processEvents();


    MainWindow w;

    QEventLoop loop;
    loop.exec();

    splash.finish(&w);//程序启动画面结束

    w.show();
    return a.exec();
}
