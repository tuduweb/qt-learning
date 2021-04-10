#ifndef NV12RENDER_H
#define NV12RENDER_H
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class YUVRender : public QOpenGLFunctions
{
public:
    YUVRender() = default;
    YUVRender(const YUVRender&) = delete;
    void initialize();
    void render(uchar*nv12Ptr, int w, int h);

private:
    QOpenGLShaderProgram program;
    GLuint idY,idUV;
    QOpenGLBuffer vbo;
};

#endif // NV12RENDER_H
