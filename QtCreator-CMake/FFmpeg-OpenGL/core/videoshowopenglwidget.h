#ifndef VIDEOSHOWOPENGLWIDGET_H
#define VIDEOSHOWOPENGLWIDGET_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#include <QOpenGLWidget>

class VideoShowOpenGLWidget : public QOpenGLWidget , protected QOpenGLFunctions
{
    Q_OBJECT
public:
    VideoShowOpenGLWidget(QWidget* parent = nullptr);
    ~VideoShowOpenGLWidget();

protected:
    void initializeGL() override;
    void paintGL() override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QOpenGLShaderProgram *program;

    QOpenGLVertexArrayObject vao;
    QOpenGLBuffer vbo;
};

#endif // VIDEOSHOWOPENGLWIDGET_H
