
// Project
#include <Qdebug>
#include "Scene.h"
#include <array>
#include <iostream>
#include <math.h>

#include <QFile>
#include <QDebug>
#include <QString>

/*
//                   P               V              M               Pos
  gl_Position = cameraToView * worldToCamera * modelToWorld * vec4(position, 1.0);

  red       QVector3D( 1.0f,  0.0f,  0.0f),
  green     QVector3D( 0.0f,  1.0f,  0.0f),
  blue      QVector3D( 0.0f,  0.0f,  1.0f),
  magenta   QVector3D( 1.0f,  0.0f,  1.0f),
  cyan      QVector3D( 0.0f,  1.0f,  1.0f),
  yellow    QVector3D( 1.0f,  1.0f,  0.0f),
 */

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
    m_myTransform.translate(0.0f, 0.0f, 0.0f);
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

void Scene::addShape()
{

}

void Scene::QtOpenGLinitialize()
{
    // create GL Buffer with SceneObject
//    Shape a;
//    a = Shape();
//    Shape b = Shape(10);

    auto _Cube1 = std::make_shared<Shape>(5);

    glEnable(GL_CULL_FACE);
    //glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    m_arcCamera.translate(0.0, 0.0, 6.0);

    // -------------create GL Shader
    m_program = new QOpenGLShaderProgram();
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/simple.vert");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/simple.frag");
    m_program->link();
    m_program->bind();


    // -------------create GL Buffers
//    m_vao.create();
//    m_vao.bind();
//    m_vvbo.create();
//    //instead of m_vvbo.create(); does the same, but for ebo the only way
//    //m_vvbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
//    m_vvbo.bind();
//    m_vvbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
//    m_vvbo.allocate(myShape, sizeof(myShape));


    //-------------create OO GL Buffers
    //qDebug()<<_Cube1->m_Id;
    _Cube1->m_vao.create();
    _Cube1->m_vao.bind();


    _Cube1->m_vvbo.create();
    //_Cube1->m_vvbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    _Cube1->m_vvbo.bind();
    _Cube1->m_vvbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    _Cube1->m_vvbo.allocate(myShape, sizeof(myShape));

    m_program->enableAttributeArray(0);
    m_program->enableAttributeArray(1);
    m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3, 2*sizeof(QVector3D));
    m_program->setAttributeBuffer(1, GL_FLOAT, sizeof(QVector3D), 3, 2*sizeof(QVector3D));

    // --------------Unbind and release all GL Shaders and GL Buffers
//    m_vao.release();
//    m_vvbo.release();

    _Cube1->m_vao.release();
    _Cube1->m_vvbo.release();
    m_program->release();

    //m_nglVAOPool[_objectName].reset(_vao);
    //m_ShapePool["Cube"].reset(_Cube1);
    std::string _test = "test";
    //qDebug()<<typeid(_Cube1).name();
    //m_ShapePool[_test].reset(_Cube1);
    // add Cube to the ShapePool
    m_ShapePool[_test] = _Cube1;
    //SceneObject _first(this, _Cube1);
    auto _first = std::make_shared<SceneObject>(this, _Cube1);
    m_SceneObjects.push_back(_first);

    //printVersionInformation();
}

void Scene::paint()
{
  //glViewport(0, 0, window()->width(), window()->height());
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  m_program->bind();
  m_program->setUniformValue("ProjectionMatrix", m_projection_matrix);
  m_program->setUniformValue("ViewMatrix", m_arcCamera.toMatrix());
  {
    //-------GL draw
//    m_vao.bind();
//    //float tx = sin(m_window->m_timer);
//    m_model_matrix.translate(0.0, 0.0, 0.0);
//    m_program->setUniformValue("ModelMatrix", m_myTransform.toMatrix());
//    glDrawArrays(GL_TRIANGLES, 0, sizeof(myShape) / sizeof(myShape[0]));
//    m_vao.release();
    //-------GL OO draw
    for(uint i = 0; i < m_SceneObjects.size() ; i++)
    {
      //qDebug()<<"enter :"<< m_SceneObjects[i]->m_Id;

      m_SceneObjects[i]->bind();
      //glDrawArrays(GL_TRIANGLES, 0, sizeof(myShape) / sizeof(myShape[0]));
      m_model_matrix.translate(0.0, 0.0, 0.0);
      m_program->setUniformValue("ModelMatrix", m_myTransform.toMatrix());
      glDrawArrays(GL_TRIANGLES, 0, sizeof(myShape) / sizeof(myShape[0]));
      m_SceneObjects[i]->release();
    }

  }
  m_program->release();
}

void Scene::update()
{
    // update from Window Qtimer animate cube
    //m_myTransform.rotate(1.0f, QVector3D(0.4f, 0.3f, 0.3f));
}

