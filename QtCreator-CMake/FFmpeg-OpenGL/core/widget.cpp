#include "widget.h"
#include "nv12render.h"
#include "videodata.h"

Widget::Widget(QWidget *parent)
{
    m_render = new Nv12Render;
    m_videoData = new VideoData(this);
    connect(m_videoData,SIGNAL(sigNewFrame()),this,SLOT(update()));
    connect(m_videoData,&VideoData::sigStarted,this,[this](uchar* p, int w, int h){
        m_ptr = p;
        m_width = w;
        m_height = h;
    });
}

Widget::~Widget()
{
    m_videoData->requestInterruption();
    m_videoData->quit();
    m_videoData->wait();
    m_videoData->deleteLater();
    delete m_render;
}

void Widget::play(QString s)
{
    m_videoData->setUrl(s);
    m_videoData->start();
}

void Widget::stop()
{
    m_videoData->requestInterruption();
}

void Widget::initializeGL()
{
    m_render->initialize();
}

void Widget::paintGL()
{
    m_render->render(m_ptr,m_width,m_height);
}
