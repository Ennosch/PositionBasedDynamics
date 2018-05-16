
#ifndef SCENE_H
#define SCENE_H

//Qt
#include <QMatrix4x4>
#include <QVector3D>
#include <QTransform>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QtGui/QOpenGLShaderProgram>

// Project
#include "AbstractScene.h"
#include "Window.h"
class Scene : public AbstractScene
{

public:
  Scene(Window *_window);
  ~Scene();
  void initialize();
  void paint();

  void QtOpenGLinitialize();
  void OpenGLinitialize();
  void update();

 // QTransform *getObject();

  float m_count = 0.0;

//private:
  //members
  int u_modelToWorld;

  QOpenGLShaderProgram* m_program;
  QOpenGLBuffer m_vvbo;
  QOpenGLVertexArrayObject m_vao;
  unsigned int shaderProgram;
  unsigned int VAO;
  unsigned int VBO;
  QMatrix4x4 m_model_matrix;
  QMatrix4x4 m_view_matrix;
  QMatrix4x4 m_projection_matrix;
  QTransform m_transform;

};

#endif // SCENE_H
