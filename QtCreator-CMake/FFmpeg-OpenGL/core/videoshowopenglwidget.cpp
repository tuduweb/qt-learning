#include "videoshowopenglwidget.h"


#include <QtDebug>


//GLSL3.0版本后,废弃了attribute关键字（以及varying关键字），属性变量统一用in/out作为前置关键字
#define GL_VERSION  "#version 330 core\n"

#define GLCHA(x)  #@x           //加单引号,将x变为字符
#define GLSTR(x)  #x            //加双引号，将x变为字符串
#define GET_GLSTR(x) GL_VERSION#x


const char *vsrc = GET_GLSTR(
    layout (location = 0) in vec3 aPos;
    void main(void)
    {
      gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    }
);

const char *fsrc =GET_GLSTR(
            out vec4 FragColor;
            void main(void)
            {
              FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
            }
);



VideoShowOpenGLWidget::VideoShowOpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{

}

VideoShowOpenGLWidget::~VideoShowOpenGLWidget()
{

}

void VideoShowOpenGLWidget::initializeGL()
{

    // 为当前环境初始化OpenGL函数
    initializeOpenGLFunctions();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);    //设置背景色为白色

    //1.创建顶点着色器
    QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    vshader->compileSourceCode(vsrc);

    //2.创建片元着色器 rgba(1.0f, 1.0f, 0.0f, 1.0f)表示黄色,而alpha值为1.0,表示完全不透明
    QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    fshader->compileSourceCode(fsrc);

    //3.创建着色器程序
    program = new QOpenGLShaderProgram;
    program->addShader(vshader);
    program->addShader(fshader);
    program->link();
    program->bind();//激活Program对象


    //4.初始化VBO,将顶点数据存储到buffer中,等待VAO激活后才能释放
    float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f
    };

    vbo.create();
    vbo.bind();              //绑定到当前的OpenGL上下文,
    vbo.allocate(vertices, 9*sizeof(GLfloat));
    vbo.setUsagePattern(QOpenGLBuffer::StreamDraw);  //设置为一次修改，多次使用


    //5.初始化VAO,设置顶点数据状态(顶点，法线，纹理坐标等)
    vao.create();
    vao.bind();

    GLint aPos = program->attributeLocation("aPos");        //获取aPos位置
    if(aPos==-1)  //未找到
    {
        return;
    }
    program->setAttributeBuffer(aPos, GL_FLOAT, 0,  3, 0);   //设置顶点属性
    program->enableAttributeArray(aPos); //使能顶点属性


    //6.解绑所有对象
    vao.release();
    vbo.release();
    program->release();

}
#include "qpainter.h"
void VideoShowOpenGLWidget::paintGL()
{

    // 绘制
   // glViewport(0, 0, width(), height());

    glClear(GL_COLOR_BUFFER_BIT);


    /***
     * 着色器，是一种较为简短的程序片段，用于告诉图形软件如何计算和输出图像。
     * shader主要分两类：Vertex Shader（顶点着色器）和Fragment Shader（片段着色器）
     */

    // 渲染Shader
    program->bind(); //绑定激活Program对象
    vao.bind();      //绑定激活vao
    glDrawArrays(GL_TRIANGLES, 0, 3);    //绘制3个定点,样式为三角形
    vao.release();       //解绑
    program->release();  //解绑

    glDisable(GL_DEPTH_TEST);
    QPainter painter;
    painter.begin(this);
    QPen pen;
    pen.setColor(Qt::red);
    painter.setPen(pen);

    painter.drawText(100,100,"helloworld");

    painter.end();
    glEnable(GL_DEPTH_TEST);

}

void VideoShowOpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << event;
}
