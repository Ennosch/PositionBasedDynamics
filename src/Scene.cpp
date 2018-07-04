#include <array>
#include <iostream>
#include <math.h>
#include <stdlib.h>

#include <Qdebug>
#include <QFile>
#include <QDebug>
#include <QString>

#include "Scene.h"

// Front Verticies
#define VERTEX_FTR QVector3D( 0.5f,  0.5f,  0.5f), QVector3D( 1.0f, 0.0f, 0.0f )
#define VERTEX_FTL QVector3D(-0.5f,  0.5f,  0.5f), QVector3D( 0.0f, 1.0f, 0.0f )
#define VERTEX_FBL QVector3D(-0.5f, -0.5f,  0.5f), QVector3D( 0.0f, 0.0f, 1.0f )
#define VERTEX_FBR QVector3D( 0.5f, -0.5f,  0.5f), QVector3D( 0.0f, 0.0f, 0.0f )

// Back Verticies
#define VERTEX_BTR QVector3D( 0.5f,  0.5f, -0.5f), QVector3D( 1.0f, 1.0f, 0.0f )
#define VERTEX_BTL QVector3D(-0.5f,  0.5f, -0.5f), QVector3D( 0.0f, 1.0f, 1.0f )
#define VERTEX_BBL QVector3D(-0.5f, -0.5f, -0.5f), QVector3D( 1.0f, 0.0f, 1.0f )
#define VERTEX_BBR QVector3D( 0.5f, -0.5f, -0.5f), QVector3D( 1.0f, 1.0f, 1.0f )


static const float quad[] = {
  -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
   1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
   1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
   1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
  -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
  -1.0f, -1.0f, 0.0f, 0.0f, 0.0f
};

static const QVector3D myShape[] = {
    // Face 1 (Front)
      VERTEX_FTR, VERTEX_FTL, VERTEX_FBL,
      VERTEX_FBL, VERTEX_FBR, VERTEX_FTR,
    // Face 2 (Back)
      VERTEX_BBR, VERTEX_BTL, VERTEX_BTR,
      VERTEX_BTL, VERTEX_BBR, VERTEX_BBL,
    // Face 3 (Top)
      VERTEX_FTR, VERTEX_BTR, VERTEX_BTL,
      VERTEX_BTL, VERTEX_FTL, VERTEX_FTR,
    // Face 4 (Bottom)
      VERTEX_FBR, VERTEX_FBL, VERTEX_BBL,
      VERTEX_BBL, VERTEX_BBR, VERTEX_FBR,
    // Face 5 (Left)
      VERTEX_FBL, VERTEX_FTL, VERTEX_BTL,
      VERTEX_FBL, VERTEX_BTL, VERTEX_BBL,
    // Face 6 (Right)
      VERTEX_FTR, VERTEX_FBR, VERTEX_BBR,
      VERTEX_BBR, VERTEX_BTR, VERTEX_FTR
};

Scene::Scene(Window *_window) : AbstractScene(_window)
{

}

Scene::~Scene()
{
}

void Scene::initialize()
{
  AbstractScene::initialize();  
  QtOpenGLinitialize();
}

void Scene::resize(int width, int height)
{
    m_projection_matrix.setToIdentity();
    m_projection_matrix.perspective(60.0f, width / float(height), 0.1f, 1000.0f);
}

void Scene::addShape(Scene *_scene, std::string _name)
{
    if(_scene->m_ShapePool[_name])
    {
        qDebug()<<_name.data()<<" already exists in ShapePool";
        return;
    }
    // create new smart pointer + instanciate new Shape
    auto pShape = std::make_shared<Shape>();
    // allocate data on the GPU
    pShape->allocate(&myShape[0], sizeof(myShape));
    // add Shape to Pool
    _scene->m_ShapePool[_name] = pShape;
}

float Scene::rand(int _mod)
{
    float r10 = std::rand() % _mod;
    float r1 = (std::rand() % _mod) / 10.0;
    //qDebug()<<"rand is: "<< r10<<"rand1 is: "<< r1 <<" result: "<<r10 * r1;
    return r10 * r1;
}

void Scene::addSceneObject(std::string _shape, const QVector3D &_pos)
{
    auto pShape = getShapeFromPool(_shape);
    if(pShape == NULL)
    {
        qDebug()<<"WARNING: COULD NOT ADD SceneObject";
        return;
    }

    auto pSO = std::make_shared<SceneObject>(this, pShape, _pos);
    m_SceneObjects.push_back(pSO);
    return;
}

std::shared_ptr<Shape> Scene::getShapeFromPool(std::string _key)
{
    ShapeMap::const_iterator got = m_ShapePool.find(_key);
    if ( got == m_ShapePool.end())
    {
        qDebug()<< "WARNING: NO SHAPE WERE FOUND";
    }
    else
    {
        auto _pShape = got->second;
        return _pShape;
    }
    return 0;
}

void Scene::QtOpenGLinitialize()
{

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    m_arcCamera.translate(0.0, 0.0, 6.0);

//----build, compline and link shaders
    m_lighting_program = new QOpenGLShaderProgram;
    m_lighting_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/screen.vert");
    m_lighting_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/screen.frag");
    m_lighting_program->link();

    m_program = new QOpenGLShaderProgram();
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/simple.vert");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/simple.frag");
    m_program->link();


//----add some shape to the ShapePool
    Scene::addShape(this, "cube");
    m_program->enableAttributeArray(0);
    m_program->enableAttributeArray(1);
    m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3, 2*sizeof(QVector3D));
    m_program->setAttributeBuffer(1, GL_FLOAT, sizeof(QVector3D), 3, 2*sizeof(QVector3D));


//----prepare a QuadPlane
    m_quad_vao = new QOpenGLVertexArrayObject(window());
    m_quad_vao->create();
    m_quad_vbo.create();
    m_quad_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_quad_vao->bind();
    m_quad_vbo.bind();
    m_quad_vbo.allocate(quad, 30 * sizeof(GLfloat));
    // tell simple shader how to interpret the quadPlane
    m_lighting_program->setAttributeBuffer("position", GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
    m_lighting_program->enableAttributeArray("position");
    m_lighting_program->setAttributeBuffer("uv", GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));
    m_lighting_program->enableAttributeArray("uv");
    m_quad_vbo.release();
    m_quad_vao->release();

//----Fill SceneObjects array
    addSceneObject("cube", QVector3D(rand(), rand(),rand()));
    addSceneObject("cube", QVector3D(rand(), rand(),rand()));
    addSceneObject("cube", QVector3D(rand(), rand(),rand()));
    addSceneObject("cube", QVector3D(rand(), rand(),rand()));
    addSceneObject("cube", QVector3D(rand(), rand(),rand()));
    addSceneObject("cube", QVector3D(rand(), rand(),rand()));
    addSceneObject("cube", QVector3D(rand(), rand(),rand()));
    addSceneObject("cube", QVector3D(rand(), rand(),rand()));
    addSceneObject("cube", QVector3D(rand(), rand(),rand()));
    addSceneObject("cube", QVector3D(rand(), rand(),rand()));
    addSceneObject("cube", QVector3D(rand(), rand(),rand()));
    addSceneObject("cube", QVector3D(rand(), rand(),rand()));
    addSceneObject("cube", QVector3D(rand(), rand(),rand()));
    addSceneObject("cube", QVector3D(rand(), rand(),rand()));
    addSceneObject("cube", QVector3D(rand(), rand(),rand()));
    addSceneObject("cube", QVector3D(rand(), rand(),rand()));

    // create a framebuffer
    m_gbuffer_fbo = new QOpenGLFramebufferObject(window()->width()*2, window()->height()*2);
    m_gbuffer_fbo->bind();
    m_gbuffer_fbo->addColorAttachment(window()->width()*2, window()->height()*2, GL_RGB);

    // create a texture the fb can render to
    m_view_position_texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    m_view_position_texture->setSize(window()->width()*2, window()->height()*2);
    m_view_position_texture->setMinificationFilter(QOpenGLTexture::Nearest);
    m_view_position_texture->setMagnificationFilter(QOpenGLTexture::Nearest);
    m_view_position_texture->setFormat(QOpenGLTexture::RGB32F);
    m_view_position_texture->allocateStorage(QOpenGLTexture::RGB, QOpenGLTexture::Float32);

    //bind the texture to the framebuffer
    glBindTexture(GL_TEXTURE_2D, m_view_position_texture->textureId());
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_view_position_texture->textureId(), 0);

    //create a render buffer
    GLuint rbo_depth;
    glGenRenderbuffers(1, &rbo_depth);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth);
    glRenderbufferStorage(
      GL_RENDERBUFFER,     // Target
      GL_DEPTH_COMPONENT,  // Internal Format
      window()->width()*2, window()->height()*2    // Dimensions
    );
    glFramebufferRenderbuffer(
      GL_FRAMEBUFFER,       // Target
      GL_DEPTH_ATTACHMENT,  // Attachment
      GL_RENDERBUFFER,      // Renderbuffer Target
      rbo_depth             // Renderbuffer
    );
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      qCritical("gBuffer FBO not complete!");

    m_gbuffer_fbo->release();

    //printVersionInformation();

}

void Scene::paint()
{
    // draw to the framebuffer (off-screen render)
//    glViewport(0,0, m_gbuffer_fbo->width(), m_gbuffer_fbo->height());
    m_gbuffer_fbo->bind();
      glEnable(GL_DEPTH_TEST);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glClearColor(0.2f, 0.0f, 0.0f, 1.0f);
      m_program->bind();
      m_program->setUniformValue("ProjectionMatrix", m_projection_matrix);
      m_program->setUniformValue("ViewMatrix", m_arcCamera.toMatrix());
      for(uint i = 0; i < m_SceneObjects.size() ; i++)
      {
        m_SceneObjects[i]->bind();
        m_program->setUniformValue("ModelMatrix", m_SceneObjects[i]->getMatrix());
        glDrawArrays(GL_TRIANGLES, 0, m_SceneObjects[i]->shape()->getVertsSize() /
                     sizeof(m_SceneObjects[i]->shape()->getData()[0]));
        m_SceneObjects[i]->release();
      }
      m_program->release();
    m_gbuffer_fbo->release();

    // draw to screen
    glViewport ( 0, 0, window()->width()*2, window()->height()*2);
    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // finally draw a quad to the screen
    m_lighting_program->bind();
    m_view_position_texture->bind(0);
    m_quad_vao->bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    m_quad_vao->release();
    m_lighting_program->release();
}

void Scene::update()
{
    // update from Window Qtimer animate cube
    //m_myTransform.rotate(1.0f, QVector3D(0.4f, 0.3f, 0.3f));
}


