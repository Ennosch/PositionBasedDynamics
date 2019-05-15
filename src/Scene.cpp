
// Project
#include <Qdebug>
#include "Scene.h"
#include <array>
#include <iostream>
#include <math.h>

#include <QFile>
#include <QDebug>
#include <QString>

#include <stdio.h>

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
    QVector3D( 0.5f,  0.5f,  0.5f),
    QVector3D( -0.5f,  0.5f,  0.5f),
    QVector3D( -0.5f,  -0.5f,  0.5f),
    QVector3D( 0.5f,  -0.5f,  0.5f),

    QVector3D( 0.5f,  0.5f,  -0.5f),
    QVector3D( -0.5f,  0.5f,  -0.5f),
    QVector3D( -0.5f,  -0.5f,  -0.5f),
    QVector3D( 0.5f,  -0.5f,  -0.5f)
};


Scene::Scene(Window *_window)
{
    m_window = _window;
    m_myTransform.translate(0.0f, 0.0f, 0.0f);
}

Scene::~Scene()
{
}

void Scene::initialize()
{

  initializeOpenGLFunctions();

  QtOpenGLinitialize();

}

void Scene::QtOpenGLinitialize()
{
    //glEnable(GL_CULL_FACE);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    m_program = new QOpenGLShaderProgram();
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/simple.vert");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/simple.frag");
    m_program->link();
    m_program->bind();

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
  m_program->bind();
  m_program->setUniformValue("ProjectionMatrix", m_projection_matrix);
  {
    m_vao.bind();
    m_model_matrix.translate(0.0, 0.0, 0.0);
    m_program->setUniformValue("ModelMatrix", m_myTransform.toMatrix());
    glDrawArrays(GL_TRIANGLES, 0, sizeof(myShape) / sizeof(myShape[0]));
    m_vao.release();
  }
  m_program->release();
}

void Scene::update()
{
    m_myTransform.rotate(1.0f, QVector3D(0.4f, 0.3f, 0.3f));
}



