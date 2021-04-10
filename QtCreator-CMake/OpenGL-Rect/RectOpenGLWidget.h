//myopenglwidget.h
#ifndef RectOpenGLWidget_H
#define RectOpenGLWidget_H
 
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
 
class QOpenGLShaderProgram;
 
class RectOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
 
public:
    explicit RectOpenGLWidget(QWidget *parent = 0);
 
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
 
private:
    QOpenGLShaderProgram *program;
    QOpenGLBuffer vbo;
};
 
#endif // MYOPENGLWIDGET_H