
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



static const QVector3D noSHape[] = {
  QVector3D( 0.0f,  0.0f,  0.0f),
   QVector3D( 1.0f,  0.0f,  0.0f),

  QVector3D( 2.0f,  5.0f, -15.0f),
   QVector3D( 0.0f,  1.0f,  0.0f),

  QVector3D(-1.5f, -2.2f, -2.5f),
   QVector3D( 0.0f,  0.0f,  1.0f),

  QVector3D(-3.8f, -2.0f, -12.3f),
   QVector3D( 1.0f,  0.0f,  1.0f),

  QVector3D( 2.4f, -0.4f, -3.5f),
    QVector3D( 0.0f,  1.0f,  1.0f),

  QVector3D(-1.7f,  3.0f, -7.5f),
    QVector3D( 0.0f,  1.0f,  1.0f),

  QVector3D( 1.3f, -2.0f, -2.5f),
    QVector3D( 1.0f,  1.0f,  0.0f),

  QVector3D( 1.5f,  2.0f, -2.5f),
    QVector3D( 1.0f,  5.0f,  2.0f),

  QVector3D( 1.5f,  0.2f, -1.5f),
    QVector3D( 2.0f,  1.0f,  3.0f),

  QVector3D(-1.3f,  1.0f, -1.5f),
    QVector3D( 0.3f,  1.0f,  2.0f)

};


/*

static const QVector3D myShape[] = {
    QVector3D( 0.5f,  0.5f,  0.5f),
    QVector3D( -0.5f,  0.5f,  0.5f),
    QVector3D( -0.5f,  -0.5f,  0.5f),
    QVector3D( 0.5f,  -0.5f,  0.5f),

    QVector3D( 0.5f,  0.5f,  -0.5f),
    QVector3D( -0.5f,  0.5f,  -0.5f)
//    QVector3D( -0.5f,  -0.5f,  -0.5f),
//    QVector3D( 0.5f,  -0.5f,  -0.5f)
};

*/

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
  //OpenGLinitialize();
  QtOpenGLinitialize();
}

void Scene::resize(int width, int height)
{
    m_projection_matrix.setToIdentity();
    m_projection_matrix.perspective(60.0f, width / float(height), 0.1f, 1000.0f);
}

void Scene::QtOpenGLinitialize()
{
    glEnable(GL_CULL_FACE);
    //glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    m_camera.translate(0.0, 0.0, -0.20);
    m_arcCamera.translate(0.0, 0.0, 6.0);

    m_program = new QOpenGLShaderProgram();
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/simple.vert");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/simple.frag");
    m_program->link();
    m_program->bind();

//    m_model_matrix.setToIdentity();
//    m_model_matrix.translate(0.0, 0.0, 0.0);
//    m_projection_matrix.setToIdentity();
//    m_transform.reset();

    u_modelToWorld = m_program->uniformLocation("ModelMatrix");
    //m_projection_matrix.perspective(45.0f, width / float(height), 0.0f, 1000.0f);

    m_vvbo.create();
    m_vvbo.bind();
    m_vvbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vvbo.allocate(myShape, sizeof(myShape));

    m_vao.create();
    m_vao.bind();
    m_program->enableAttributeArray(0);
    m_program->enableAttributeArray(1);
    m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3, 2*sizeof(QVector3D));
    m_program->setAttributeBuffer(1, GL_FLOAT, sizeof(QVector3D), 3, 2*sizeof(QVector3D));

    m_vao.release();
    m_vvbo.release();
    m_program->release();

    //printVersionInformation();

}

void Scene::paint()
{
  //glViewport(0, 0, window()->width(), window()->height());
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  /*
  //                   P               V              M               Pos
    gl_Position = cameraToView * worldToCamera * modelToWorld * vec4(position, 1.0);


    modelToWorld = M
    worldToView = P
   */
  m_program->bind();
  m_program->setUniformValue("ProjectionMatrix", m_projection_matrix);
  //qDebug()<<m_camera;
  //m_program->setUniformValue("ViewMatrix", m_camera);
  //m_program->setUniformValue("ViewMatrix", m_arcCamera.toMatrix());
  //m_arcCamera.reset();
  m_program->setUniformValue("ViewMatrix", m_arcCamera.toMatrix());

  //m_program->setUniformValue("ViewMatrix", m_camera);
  {
    m_vao.bind();

    //float tx = sin(m_window->m_timer);
    m_model_matrix.translate(0.0, 0.0, 0.0);
    m_program->setUniformValue("ModelMatrix", m_myTransform.toMatrix());
    //m_program->setUniformValue(u_modelToWorld, m_model_matrix);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(myShape) / sizeof(myShape[0]));
    m_vao.release();
  }
  m_program->release();
}

void Scene::update()
{
    // update from Window Qtimer
    // (?)  adding to QTransform m_transform, moves the
    //m_count += 0.05;
    //m_transform.translate(m_count , 0.0);
    //m_myTransform.rotate(1.0f, QVector3D(0.4f, 0.3f, 0.3f));
    //qDebug()<<"update";

}

void Scene::foo()
{
    qDebug("Scene foo");
}

