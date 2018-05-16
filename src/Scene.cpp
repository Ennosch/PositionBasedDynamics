
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

 */


/*
static const QVector3D myShape[] = {
    QVector3D( 0.5f,  0.5f,  0.5f),
    QVector3D( -0.5f,  0.5f,  0.5f),
    QVector3D( -0.5f,  -0.5f,  0.5f),
    QVector3D( 0.5f,  -0.5f,  0.5f),

    QVector3D( 0.5f,  0.5f,  -0.5f),
    QVector3D( -0.5f,  0.5f,  -0.5f),
    QVector3D( -0.5f,  -0.5f,  -0.5f),
    QVector3D( 0.5f,  -0.5f,  -0.5f)
};*/



static const QVector3D myShape[] = {
    QVector3D( 0.0f,  0.75f,  1.0f),
    QVector3D( 1.0f,  0.0f,  0.0f),

    QVector3D( -0.75f,  -0.75f,  1.0f),
    QVector3D( 0.0f,  1.0f,  0.0f),

    QVector3D( 0.75f,  -0.75f,  1.0f),
    QVector3D( 0.0f,  0.0f,  1.0f)
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
  //OpenGLinitialize();
  QtOpenGLinitialize();
}

void Scene::QtOpenGLinitialize()
{
    glEnable(GL_CULL_FACE);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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
    m_myTransform.rotate(1.0f, QVector3D(0.4f, 0.3f, 0.3f));
    paint();
    //qDebug()<<"update";

}


