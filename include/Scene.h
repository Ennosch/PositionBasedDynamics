
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



  float m_count = 0.0;

private:
  friend class Window;
  int u_modelToWorld;

  // to cache location the uniform vars
  int u_ModelMatrix;
  int u_ViewMatrix;
  int u_ProjectionMatrix;

  QOpenGLShaderProgram* m_program;
  QOpenGLBuffer m_vvbo;
  QOpenGLVertexArrayObject m_vao;


  unsigned int shaderProgram;
  unsigned int VAO;
  unsigned int VBO;

  QMatrix4x4 m_model_matrix;
  QMatrix4x4 m_view_matrix;
  QMatrix4x4 m_projection_matrix;

  QMatrix4x4 m_camera;
  Camera3D m_arcCamera;

  QTransform m_transform;

  Transform m_myTransform;

  void foo();

  QVector3D m_focus = QVector3D(0.0, 0.0, 0.0);

  inline void moveRightFocus(){ m_focus += QVector3D(0.001, 0.0, 0.0); };

};

#endif // SCENE_H
