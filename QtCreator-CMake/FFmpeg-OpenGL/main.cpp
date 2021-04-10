#include "mainwindow.h"

#include <QApplication>

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
}

#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    std::cout << av_version_info() << std::endl;
    w.show();
    return a.exec();
}
