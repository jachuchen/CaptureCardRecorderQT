#include "PlayWidget_yuy2.h"
//#include "recordercore.h"
#include <QOpenGLTexture>
//qt5 without qt6 install
//#include <QOpenGLVersionFunctionsFactory>




#include <QOpenGLBuffer>
#include <QMouseEvent>
#include <QThread>
#include <QTimer>

CPlayWidget_yuy2::CPlayWidget_yuy2 (QWidget *parent) : QOpenGLWidget(parent)
{
    imgheight = 1080;
    imgwidth = 1920;

                    m_pBufYuvHeightp = new unsigned char[1920*1080*2];
//                   FILE *f = fopen("c:\\temp\\fullhd.yuy2","rb");
//                   fread(m_pBufYuvHeightp,1920*1080*2,sizeof(BYTE),f) ;
//                   fclose(f);

    //setSurfaceType(QWindow::OpenGLSurface);

    connect(this, &CPlayWidget_yuy2::updateUi, this, &CPlayWidget_yuy2::onUpdateUi);
    auto timer = new QTimer(this);
            QObject::connect(timer, &QTimer::timeout, [&] (){
                update();
            } );
            timer->start(60);
}

CPlayWidget_yuy2::~CPlayWidget_yuy2()
{

    if (m_pBufYuvHeightp)
        delete m_pBufYuvHeightp;
}

void CPlayWidget_yuy2::PlayOneFrame(char* pFrameBuffer, ulong nFrameBufferLen)
{

    qDebug() << "play one....";
    int nLen = nFrameBufferLen;

    memcpy(m_pBufYuvHeightp,pFrameBuffer,nLen);

    emit updateUi();

    return;
}

void CPlayWidget_yuy2::onUpdateUi()
{

//    if (!isFullScreen())
//        showFullScreen();
    update();

}

QByteArray versionedShaderCode(const char *src)
{
    QByteArray versionedSrc;

    if (QOpenGLContext::currentContext()->isOpenGLES())
        versionedSrc.append(QByteArrayLiteral("#version 300 es\n"));
    else
        versionedSrc.append(QByteArrayLiteral("#version 330\n"));

    //    versionedSrc.append(QByteArrayLiteral("#version 150 compatibility"));

    versionedSrc.append(src);
    return versionedSrc;
}


static QVector4D color_vec0(1.16438353f, 0.00000000f, 1.79274106f, -0.972945154f);
static QVector4D color_vec1(1.16438353f, -0.213248610f, -0.532909334f, 0.301482677f);
static QVector4D color_vec2(1.16438353f, 2.11240172f, 0.f, -1.13340223f);
static QVector3D color_range_min(0.0627451017f, 0.0627451017f, 0.0627451017f);
static QVector3D color_range_max(0.921568632f, 0.941176474f, 0.941176474f);

static float vertex_coord_data[] = {
    -1.f, -1.f, 0.f,   0.f, 1.f,
    -1.f,  1.f, 0.f,   0.f, 0.f,
    1.f,  1.f, 0.f,   1.f, 0.f,
    1.f, -1.f, 0.f,   1.f, 1.f,
};

static uint32_t vertx_index_data[] = {
    0, 1, 2,
    0, 3, 2
};

static const char *vsrc = R"(

layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in vec2 tex_pos;

out vec2 tex_uv;

void main()
{
    gl_Position = vec4(vertex_pos, 1.f);
    tex_uv = vec2(tex_pos);
}
        )";



static const char *fsrc = R"(

out vec4 frage_color;

in vec2 tex_uv;

uniform sampler2D tex_yuyv;


uniform vec4 color_vec0;
uniform vec4 color_vec1;
uniform vec4 color_vec2;

uniform vec3 color_range_min;
uniform vec3 color_range_max;

void main()
{
    vec3 yuv = vec3(0.f);

    yuv.xy = texture(tex_yuyv, tex_uv).xy;
    yuv.z = texture(tex_yuyv, tex_uv).w;

    yuv = clamp(yuv, color_range_min, color_range_max);

    vec3 rgb = vec3(0.f);
    rgb.r = dot(color_vec0.xyz, yuv) + color_vec0.w;
    rgb.g = dot(color_vec1.xyz, yuv) + color_vec1.w;
    rgb.b = dot(color_vec2.xyz, yuv) + color_vec2.w;

    frage_color = vec4(rgb, 1.f);
}
                                 )";



void CPlayWidget_yuy2::initializeGL()
{


    initializeOpenGLFunctions();
    glClearColor(0.0, 0.0, 0.0, 1.0);

    m_program = new QOpenGLShaderProgram;


    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, versionedShaderCode(vsrc));
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, versionedShaderCode(fsrc));




    //QOpenGLContext *context = QOpenGLContext::currentContext();

    // bind location for the vertex shader
    m_program->bindAttributeLocation("vertex_pos", 0);

    m_program->link();
    m_program->bind();
    //qt5 without qt6 install

    //funcs = QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_3_3_Core>(QOpenGLContext::currentContext());


    funcs = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();

    if (!funcs) {
        qWarning() << "Could not obtain required OpenGL context version";
        exit(1);
    }

    //makeCurrent();
    funcs->glGenBuffers(1, &VBO);
    funcs->glGenBuffers(1, &EBO);

    funcs->glGenVertexArrays(1, &VAO);
    funcs->glBindVertexArray(VAO);
    funcs->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    funcs->glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_coord_data), vertex_coord_data, GL_STATIC_DRAW);

    funcs->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    funcs->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertx_index_data), vertx_index_data, GL_STATIC_DRAW);
    funcs->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);

    funcs->glEnableVertexAttribArray(0);

    funcs->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    funcs->glEnableVertexAttribArray(1);

    m_program->setUniformValue("color_range_min",color_range_min);
    m_program->setUniformValue("color_range_max",color_range_max);

    m_program->setUniformValue("color_vec0",color_vec0);
    m_program->setUniformValue("color_vec1",color_vec1);
    m_program->setUniformValue("color_vec2",color_vec2);


    funcs->glGenTextures(1, &m_tex_yuyv);
    funcs->glBindTexture(GL_TEXTURE_2D, m_tex_yuyv);
    funcs->glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    funcs->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgwidth/2, imgheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    funcs->glGenerateMipmap(GL_TEXTURE_2D);

}
void CPlayWidget_yuy2::resizeGL(int w, int h)
{
    if(h == 0)// 防止被零除
    {
        h = 1;// 将高设为1
    }
    //设置视口
    glViewport(0,0, w,h);


}

void CPlayWidget_yuy2::paintGL()
{


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    {
        funcs->glActiveTexture(GL_TEXTURE0);
        funcs->glBindTexture(GL_TEXTURE_2D, m_tex_yuyv);
        funcs->glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, imgwidth/2 , imgheight, GL_RGBA, GL_UNSIGNED_BYTE, m_pBufYuvHeightp);

        funcs->glActiveTexture(GL_TEXTURE0);
        funcs->glBindTexture(GL_TEXTURE_2D, m_tex_yuyv);
        funcs->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }


    update();

    return;


}


