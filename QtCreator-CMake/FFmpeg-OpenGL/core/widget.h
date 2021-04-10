#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
QT_FORWARD_DECLARE_CLASS(Nv12Render)
QT_FORWARD_DECLARE_CLASS(VideoData)
class Widget : public QOpenGLWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    void play(QString);
    void stop();

protected:
    void initializeGL() override;
    void paintGL() override;

private:
    Nv12Render *m_render;
    uchar *m_ptr;
    int m_width,m_height;
    VideoData *m_videoData;
};

#endif // WIDGET_H
