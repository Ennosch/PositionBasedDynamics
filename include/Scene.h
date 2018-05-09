
#ifndef SCENE_H
#define SCENE_H

//Qt
#include <QMatrix4x4>
#include <QVector3D>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QtGui/QOpenGLShaderProgram>


// Project
#include "AbstractScene.h"
#include "Window.h"
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QtGui/QOpenGLShaderProgram>

////////////////////////////////////////////////////////////////////////////////
/// @class Scene
/// @brief This is an example on how to subclass the AbstractScene and use your
/// OpenGL commands.
///
/// In order to create your scene you need to subclass the AbstractScene through
/// public inheritance and write initialize() and paint(). Both are virtual, but
/// the paint() is pure virtual, initialize() is not.
///
/// There is just one requirement, AbstractScene::initialize() method must be
/// called from the Scene::initialize() in order to initialize the OpenGL
/// functions, that saves you from having to do it. Just make sure you call
/// the superclass method and you are good to go.
////////////////////////////////////////////////////////////////////////////////
class Scene : public AbstractScene
{

public:
  Scene(Window *_window);
  ~Scene();
  void initialize();
  void paint();

  void QtOpenGLinitialize();
  void OpenGLinitialize();

//private:
  //members
  QOpenGLShaderProgram* m_program;
  QOpenGLBuffer m_vvbo;
  QOpenGLVertexArrayObject m_vao;
  unsigned int shaderProgram;
  unsigned int VAO;
  unsigned int VBO;
  QMatrix4x4 m_model_matrix;
  QMatrix4x4 m_view_matrix;
  QMatrix4x4 m_projection_matrix;

};

#endif // SCENE_H
