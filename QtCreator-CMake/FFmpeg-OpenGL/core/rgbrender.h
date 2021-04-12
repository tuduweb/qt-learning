#ifndef RGBRender_H
#define RGBRender_H
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class YUVRender : public QOpenGLFunctions
{
public:
    YUVRender() = default;
    YUVRender(const YUVRender&) = delete;
    void initialize();
    void render(uchar*yuy2Ptr, int w, int h);

private:
    QOpenGLShaderProgram program;
	GLuint idRGB;
    QOpenGLBuffer vbo;
};

#endif // NV12RENDER_H
