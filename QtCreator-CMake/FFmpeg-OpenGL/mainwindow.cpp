#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "core/remotestreaminterface.h"
#include "core/videoshowopenglwidget.h"

#include "core/widget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    //VideoShowOpenGLWidget* openglWidget = new VideoShowOpenGLWidget(this);

    Widget* openglWidget = new Widget(this);
    ui->mainLayout->addWidget(openglWidget);

    ui->mainLayout->setStretch(0, 0);
    ui->mainLayout->setStretch(1, 1);

    //openglWidget->play("./cat.mp4");

    openglWidget->play("rtsp://admin:admin123@172.28.140.89:554/h264/ch1/main/av_stream");
    //openglWidget->play("rtsp://localhost:8554/mystream");
}

MainWindow::~MainWindow()
{
    delete ui;
}

