////////////////////////////////////////////////////////////////////////////////
/// @file Scene.cpp
/// @author Ramon Blanquer
/// @version 0.0.1
////////////////////////////////////////////////////////////////////////////////

// Project
#include "Scene.h"
#include <array>
#include <iostream>

#include <QFile>
#include <QDebug>
/*
#include &lt;QtGui/QOpenGLContext&gt;
#include &lt;QtGui/QOpenGLBuffer&gt;
#include &lt;QtGui/QOpenGLVertexArrayObject&gt;
*/
Scene::Scene(Window *_window) : AbstractScene(_window)
{
}

Scene::~Scene()
{
}

void Scene::initialize()
{
  AbstractScene::initialize();
  OpenGLinitialize();
}

void Scene::QtOpenGLinitialize()
{
    m_program = new QOpenGLShaderProgram();
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":shader/simple.vert");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/simple.frag");
    m_program->link();

    GLfloat verts_pos[] ={
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };
    //QOpenGLBuffer m_vvbo(QOpenGLBuffer::VertexBuffer);
    m_vao.create();
    m_vao.bind();
    m_vvbo.create();
    m_vvbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vvbo.bind();
    m_vvbo.allocate(verts_pos, sizeof(verts_pos));
    std::cout<<"log: "<< sizeof(verts_pos);

    m_program->enableAttributeArray(0);
    m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3);

    m_vvbo.release();
    m_vao.release();
}

void Scene::OpenGLinitialize()
{
    GLfloat vertices[] ={
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    QString test(":/simple.vert");
    qDebug()<<test<<endl;
}

void Scene::paint()
{
  glViewport(0, 0, window()->width(), window()->height());
  glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  // render using the Qt classes
  m_vao.bind();
  glDrawArrays(GL_TRIANGLES, 0, 3);
  m_vao.release();
  // how to purely render a VBO
  /*
  m_vvbo.bind();
  m_program->bind();
  glDrawArrays(GL_TRIANGLES, 0, 3);
  m_vvbo.release();
  m_program->release();*/

}
