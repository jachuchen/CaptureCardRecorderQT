#ifndef CPLAYWIDGET_YUY2
#define CPLAYWIDGET_YUY2
#include <QOpenGLWidget>
#include <QOpenGLWindow>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QFile>
#include <QElapsedTimer>
#define ATTRIB_VERTEX 3
#define ATTRIB_TEXTURE 4


//#ifdef __cplusplus
//extern "C" {
//#endif

//#include <libavcodec/avcodec.h>
//#include <libavformat/avformat.h>
//#include <libavutil/imgutils.h>
//#include <libswscale/swscale.h>
//#include <libavdevice/avdevice.h>

//#ifdef __cplusplus
//}
//#endif


class CPlayWidget_yuy2: public QOpenGLWidget, protected QOpenGLFunctions
// public  QOpenGLWindow
{
    Q_OBJECT
public:
    unsigned char* m_pBufYuvHeightp =NULL;
    CPlayWidget_yuy2 (QWidget *parent);
    ~CPlayWidget_yuy2();
    void PlayOneFrame(char* pFrameBuffer, ulong nFrameBufferLen);
signals:
    void updateUi();
public slots:
    void  onUpdateUi();
protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
private:
    uint   imgwidth;
    uint   imgheight;

    FILE* m_pYuvFile = NULL;

    QOpenGLShaderProgram* m_program;
    //QOpenGLFunctions *f;
    QOpenGLFunctions_3_3_Core* funcs = nullptr;

    GLuint  VBO,VAO ,EBO;
    //    QOpenGLVertexArrayObject vaoQuad;
    //    QOpenGLBuffer vboQuad;

    GLuint		  m_tex_yuyv;
    QOpenGLTexture *texture;

private:
//    AVFormatContext* formatContext = nullptr;
//    AVCodecContext* codecContext = nullptr;
//    AVFrame* frame = nullptr;
//    const AVCodec* codec = nullptr;
//    int videoStreamIndex = -1;
//    uint8_t* buffer;

//    AVPacket packet;
//    GLuint textureID;
//    int initFFmpeg();
//    bool readFrame();

};
#endif

