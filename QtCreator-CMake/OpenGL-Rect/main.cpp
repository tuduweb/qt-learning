//main.cpp
#include <QApplication>
#include "myopenglwidget.h"
#include "RectOpenGLWidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);
    MyOpenGLWidget w;
    w.resize(400, 300);
    w.show();

    RectOpenGLWidget r;
    r.resize(600, 300);
    r.show();
    return app.exec();
}