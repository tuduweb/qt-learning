#include "nv12render.h"
#include <QOpenGLTexture>
#include <QDebug>

void Nv12Render::initialize()
{
    initializeOpenGLFunctions();
    const char *vsrc =
            "attribute vec4 vertexIn; \
             attribute vec4 textureIn; \
             varying vec4 textureOut;  \
             void main(void)           \
             {                         \
                 gl_Position = vertexIn; \
                 textureOut = textureIn; \
             }";

    const char *fsrc =
            "varying mediump vec4 textureOut;\n"
            "uniform sampler2D textureY;\n"
            "uniform sampler2D textureUV;\n"
            "void main(void)\n"
            "{\n"
                "vec3 yuv; \n"
                "vec3 rgb; \n"
                "yuv.x = texture2D(textureY, textureOut.st).r - 0.0625; \n"
                "yuv.y = texture2D(textureUV, textureOut.st).r - 0.5; \n"
                "yuv.z = texture2D(textureUV, textureOut.st).g - 0.5; \n"
                "rgb = mat3( 1,       1,         1, \n"
                            "0,       -0.39465,  2.03211, \n"
                            "1.13983, -0.58060,  0) * yuv; \n"
                "gl_FragColor = vec4(rgb, 1); \n"
            "}\n";
    //gl_FragColor ->代表画面所要填入的颜色->输出的颜色用于随后的像素操作


    /**
    vertex shader（顶点着色器）：vertex shader在每个顶点上都执行执行一次，通过不同世界的坐标系转化定位顶点的最终位置。它可以数据给fragment shader，如纹理坐标、顶点坐标，变换矩阵等。
    fragment shader（片段着色器）：fragment shader在每个像素上都会执行一次，通过插值确定像素的最终显示颜色。
    **/

    program.addCacheableShaderFromSourceCode(QOpenGLShader::Vertex,vsrc);
    program.addCacheableShaderFromSourceCode(QOpenGLShader::Fragment,fsrc);
    program.link();

    GLfloat points[]{
        -1.0f, 1.0f,
         1.0f, 1.0f,
         1.0f, -1.0f,
        -1.0f, -1.0f,

        0.0f,0.0f,
        1.0f,0.0f,
        1.0f,1.0f,
        0.0f,1.0f
    };

    vbo.create();
    vbo.bind();
    vbo.allocate(points,sizeof(points));

    GLuint ids[2];
    glGenTextures(2,ids);
    idY = ids[0];
    idUV = ids[1];
}

void Nv12Render::render(uchar *nv12Ptr, int w, int h)
{
    if(!nv12Ptr)return;

    glClearColor(0.5f, 0.5f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    program.bind();
    vbo.bind();
    program.enableAttributeArray("vertexIn");
    program.enableAttributeArray("textureIn");
    program.setAttributeBuffer("vertexIn",GL_FLOAT, 0, 2, 2*sizeof(GLfloat));
    program.setAttributeBuffer("textureIn",GL_FLOAT,2 * 4 * sizeof(GLfloat),2,2*sizeof(GLfloat));

    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D,idY);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RED,w,h,0,GL_RED,GL_UNSIGNED_BYTE,nv12Ptr);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D,idUV);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RG,w >> 1,h >> 1,0,GL_RG,GL_UNSIGNED_BYTE,nv12Ptr + w*h);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    program.setUniformValue("textureUV",0);
    program.setUniformValue("textureY",1);
    glDrawArrays(GL_QUADS,0,4);
    program.disableAttributeArray("vertexIn");
    program.disableAttributeArray("textureIn");
    program.release();
}
