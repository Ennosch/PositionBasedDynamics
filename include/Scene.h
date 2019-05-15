
#ifndef SCENE_H
#define SCENE_H

//Qt
#include <QMatrix4x4>
#include <QVector3D>
#include <QTransform>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QtGui/QOpenGLShaderProgram>

#include <QOpenGLFunctions>

// Project
#include "Window.h"
#include "transform.h"

class Scene : public QOpenGLFunctions
{

public:
  Scene(Window *_window);
  ~Scene();
  void initialize();
  void paint();

  void QtOpenGLinitialize();
  void update();

  QOpenGLWindow *m_window = nullptr;

private:
  friend class Window;

  QOpenGLShaderProgram* m_program;
  QOpenGLBuffer m_vvbo;
  QOpenGLVertexArrayObject m_vao;
  QMatrix4x4 m_model_matrix;
  QMatrix4x4 m_view_matrix;
  QMatrix4x4 m_projection_matrix;

  Transform m_myTransform;

};

#endif // SCENE_H
