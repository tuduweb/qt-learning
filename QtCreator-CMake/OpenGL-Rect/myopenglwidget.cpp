//myopenglwidget.cpp
#include "myopenglwidget.h"
#include <QOpenGLShaderProgram>
 
MyOpenGLWidget::MyOpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
}
 
void MyOpenGLWidget::initializeGL()
{
    // 为当前环境初始化OpenGL函数
    initializeOpenGLFunctions();
 
    // 创建顶点着色器
    QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    const char *vsrc =
            "attribute vec4 vPosition;                        \n"
            "attribute vec4 vColor;                           \n"
            "varying vec4 color;                           \n"
            "void main() {                             \n"
            "   color = vColor;                        \n"
            "   gl_Position = vPosition;               \n"
            "}                                         \n";
    vshader->compileSourceCode(vsrc);
    // 创建片段着色器
    QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    const char *fsrc =
            "varying vec4 color;                             \n"
            "void main() {                              \n"
            "   gl_FragColor = color;                         \n"
            "}                                          \n";
    fshader->compileSourceCode(fsrc);
 
    // 创建着色器程序
    program = new QOpenGLShaderProgram;
    program->addShader(vshader);
    program->addShader(fshader);
 
    program->link();
    program->bind();
}
 
void MyOpenGLWidget::resizeGL(int , int )
{  
}
 
void MyOpenGLWidget::paintGL()
{
    int w = width();
    int h = height();
    int side = qMin(w, h);
    //glViewport((w - side) / 2, (h - side) / 2, side, side);
    //glViewport(w, h, side, side);
 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
    // 顶点位置
    // GLfloat vertices[] = {
    //     -1.0f, 1.0f,
    //     -0.8f, -0.8f,
    //     0.8f, -0.8f,
    //     0.8f, 0.8f
    // };
    GLfloat vertices[] = {
        -0.5f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.5f, 0.5f , 0.0f
    };
 
    vbo.create();
    vbo.bind();
    vbo.allocate(vertices, 24*sizeof(GLfloat));
 
    GLuint vPosition = program->attributeLocation("vPosition");
    program->setAttributeBuffer(vPosition, GL_FLOAT, 0, 3, 3 * sizeof(GLfloat));
    glEnableVertexAttribArray(vPosition);
 
 
    // 顶点颜色
    GLfloat colors[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,//右下
        1.0f, 1.0f, 1.0f//右上
    };
    vbo.write(12*sizeof(GLfloat), colors, 12*sizeof(GLfloat));
    GLuint vColor = program->attributeLocation("vColor");
    program->setAttributeBuffer(vColor, GL_FLOAT, 12*sizeof(GLfloat), 3, 3 * sizeof(GLfloat));
    //offset:第一个数据的偏移量
    //tupleSize:一个数据有多少个元素,比如位置为xyz,颜色为rgb,所以是3
    //stride:步长,下个数据距离当前数据的之间距离,比如右下位置和左下位置之间间隔了:3个xyz值+3个rgb值,所以填入 6 * sizeof(float)
    glEnableVertexAttribArray(vColor);
 
    // 绘制
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

}