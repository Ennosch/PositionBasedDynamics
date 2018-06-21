
#ifndef SCENE_H
#define SCENE_H

// STL
#include <vector>
#include <memory>
#include <unordered_map>

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
#include "sceneobject.h"
#include "shape.h"

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
  void addShape();

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

  std::vector <std::shared_ptr <SceneObject>> m_SceneObjects;
  std::unordered_map <std::string, std::shared_ptr <Shape>> m_ShapePool;


};

#endif // SCENE_H

/*
 *
 * WIP for Obejct pool
/// @brief A container for all the objs we can draw
std::unordered_map<std::string, ngl::Obj* > m_nglObjPool;

/// @brief A container for all the VAOs we can draw
std::unordered_map<std::string, std::shared_ptr<ngl::AbstractVAO> > m_nglVAOPool;
*/
