#include "nv12render.h"
#include <QOpenGLTexture>
#include <QDebug>

void Nv12Render::initialize()
{
    initializeOpenGLFunctions();
    /**
      * varying 纹理坐标 会自动插值
     **/
    const char *vsrc =
            "attribute vec4 vertexIn; \
             attribute vec4 textureIn; \
             varying vec4 textureOut;  \
             void main(void)           \
             {                         \
                 gl_Position = vertexIn; \
                 textureOut = textureIn; \
             }";

    /***
     * sampler2D textureY 包含我们将会使用的纹理单元
     * 再通过texture2D函数我们可以得到一个纹素(texel)，这是一个纹理图片中的像素,参数分别为纹理单元和纹理坐标
     * // shader 会将数据归一化，而 uv 的取值区间本身存在-128到正128 然后归一化到0-1 为了正确计算成rgb，
     * // 则需要归一化到 -0.5 - 0.5的区间
     */

    /***之前我们渲染的三角形只能固定设置一种颜色,比如我们想通过应用程序中根据不同情况来发送我们想渲染的颜色,该怎么办？使用uniform变量
    3.1 Uniform
    Uniform是一种从CPU中的应用向GPU中的（vertex和fragment）着色器发送数据的方式，但uniform和顶点属性有些不同。
    首先，uniform是全局的(Global)。全局意味着uniform变量必须在每个着色器程序对象中都是独一无二的，而且它可以被着色器程序的任意着色器在任意阶段访问。它不能被shader程序修改.（shader只能用，不能改，只能等外部程序重新重置或更新),Uniform变量通过application调用函数glUniform()函数赋值的.
    而glUniform()函数分为很多种,因为OpenGL由C语言编写，但是C语言不支持函数重载，所以会有很多名字相同后缀不同的函数,glUniform大概格式为 :
    */

    //mediump  – 16位浮点格式，适用于纹理UV坐标和比highp 大约快两倍
    const char *fsrc =
            "varying mediump vec4 textureOut;\n"
            "uniform sampler2D textureY;\n"
            "uniform sampler2D textureUV;\n"
            "uniform int formatType;\n"
            "void main(void)\n"
            "{\n"
                "vec3 yuv; \n"
                "vec3 rgb; \n"
                "yuv.x = texture2D(textureY, textureOut.st).r - 0.0625; \n"
                "yuv.y = texture2D(textureUV, textureOut.st).r - 0.5; \n"
                "yuv.z = texture2D(textureUV, textureOut.st).a - 0.5; \n"
                "rgb = mat3( 1,       1,         1, \n"
                            "0,       -0.39465,  2.03211, \n"
                            "1.13983, -0.58060,  0) * yuv; \n"
                "gl_FragColor = vec4(rgb, 1); \n"
            "}\n";
    //gl_FragColor ->代表画面所要填入的颜色->输出的颜色用于随后的像素操作////输出像素值给光栅器
    //vec4(rgb, 1) //->片段fragment的颜色,alpha值

    /**
    vertex shader（顶点着色器）：vertex shader在每个顶点上都执行执行一次，通过不同世界的坐标系转化定位顶点的最终位置。它可以数据给fragment shader，如纹理坐标、顶点坐标，变换矩阵等。
    fragment shader（片段着色器）：fragment shader在每个像素上都会执行一次，通过插值确定像素的最终显示颜色。
    **/

    program.addCacheableShaderFromSourceCode(QOpenGLShader::Vertex,vsrc);
    program.addCacheableShaderFromSourceCode(QOpenGLShader::Fragment,fsrc);
    program.link();

    //以下注释为盲猜 不一定正确 因为当前程序绘制的四边形,2d下一共4个点 那么就是4个顶点
    GLfloat points[]{
        //顶点信息
        -1.0f, 1.0f,
         1.0f, 1.0f,
         1.0f, -1.0f,
        -1.0f, -1.0f,
        //颜色信息?
        0.0f,0.0f,
        1.0f,0.0f,
        1.0f,1.0f,
        0.0f,1.0f
    };

    vbo.create();
    vbo.bind();
    vbo.allocate(points,sizeof(points));

    GLuint ids[2];//纹理对象数组，数组里面是纹理对象的id
    glGenTextures(2,ids);//创建两个纹理对象。
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

    //被attribute修饰的变量是只能在vertex shader中使用的变量。attribute表示一些顶点的数据比如：顶点坐标，法线，纹理坐标，顶点颜色等。
    //location, type, offset, tupleSize, 一个顶底的所有字节数
    program.setAttributeBuffer("vertexIn",GL_FLOAT, 0, 2, 2*sizeof(GLfloat));
    program.setAttributeBuffer("textureIn",GL_FLOAT,2 * 4 * sizeof(GLfloat),2,2*sizeof(GLfloat));
    //激活第1层纹理,绑定到创建的opengl纹理
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D,idY);//绑定纹理对象到纹理目标GL_TEXTURE_2D上，接下来对纹理目标的操作都发生在此对象上
    //QOpenGLFunctions::glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels)
    //创建纹理图像 采样工作在..进行 图形数据存储在绑定的对象中
    glTexImage2D(GL_TEXTURE_2D,
                 0,//细节基本 0默认
                 GL_LUMINANCE,//gpu内部格式 亮度 灰度图
                 w,//拉伸到全屏
                 h,//.
                 0,//边框
                 GL_LUMINANCE,//数据的像素格式 亮度,灰度图 要与上面一致
                 GL_UNSIGNED_BYTE,//像素的数据类型
                 nv12Ptr//纹理数据
                 );

    //设置放大和缩小时，纹理的过滤选项为：线性过滤
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    //设置纹理X,Y轴的纹理环绕选项为：边缘像素延伸
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //激活第0层纹理，绑定到创建的opengl纹理
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D,idUV);
    //w>>1,h>>1大小相比缩了一半 这样u/v分到了r,a上
    glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE_ALPHA,w >> 1,h >> 1,0,GL_LUMINANCE_ALPHA,GL_UNSIGNED_BYTE,nv12Ptr + w*h);
    //    glTexImage2D(GL_TEXTURE_2D,0,GL_RG,w >> 1,h >> 1,0,GL_RG,GL_UNSIGNED_BYTE,nv12Ptr + w*h);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    //一致变量（Uniform Variables）
    //一致变量。在着色器执行期间一致变量的值是不变的。与const常量不同的是，这个值在编译时期是未知的是由着色器外部初始化的。一致变量在顶点着色器和片段着色器之间是共享的。它也只能在全局范围进行声明。
    program.setUniformValue("textureUV",0);//大概是设置纹理到变量?
    program.setUniformValue("textureY",1);
    program.setUniformValue("formatType",33);//传到了opengl中, cpu->gpu
    //program.setUniformValue("test", 0.5f);//传到了opengl中, cpu->gpu
    glDrawArrays(GL_QUADS,0,4);
    program.disableAttributeArray("vertexIn");
    program.disableAttributeArray("textureIn");

    ////解绑//自己尝试添加的
    //glBindTexture(GL_TEXTURE_2D, 0);

    program.release();
}
