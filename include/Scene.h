
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
#include "transform.h"
#include "camera.h"
#include "vector"

class Scene : public AbstractScene
{

public:
  Scene(Window *_window);
  ~Scene();
  void initialize();
  void resize(int width, int height);
  void paint();

  void QtOpenGLinitialize();
  void update();

private:
  friend class Window;

  // to cache location the uniform vars

  QOpenGLShaderProgram* m_program;
  QOpenGLBuffer m_vvbo;
  QOpenGLVertexArrayObject m_vao;

  QMatrix4x4 m_model_matrix;
  QMatrix4x4 m_view_matrix;
  QMatrix4x4 m_projection_matrix;

  Camera3D m_arcCamera;
  Transform m_myTransform;




};

#endif // SCENE_H
