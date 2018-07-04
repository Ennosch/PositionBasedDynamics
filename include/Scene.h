
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
#include <QOpenGLFramebufferObject>
#include <QOpenGLFramebufferObjectFormat>
#include <QOpenGLTexture>

// Project
#include "AbstractScene.h"
#include "Window.h"
#include "transform.h"
#include "camera.h"
#include "sceneobject.h"
#include "shape.h"


typedef std::unordered_map <std::string, std::shared_ptr <Shape>> ShapeMap;

class Scene : public AbstractScene
{

public:
  Scene(Window *_window);
  ~Scene();
  void initialize();
  void resize(int width, int height);
  void paint();
  void update();

  void QtOpenGLinitialize();

  void addSceneObject(std::string _shape);
  void addSceneObject(std::string _shape, const QVector3D &_pos);
  static void addShape(Scene *_scene, std::string _name);
  float rand(int _mod = 10);

  std::shared_ptr<Shape> getShapeFromPool(std::string _key);


private:
  friend class Window;

  // to cache location the uniform vars

  int m_Id = 5;
  static int m_ShapeCount;

  QOpenGLShaderProgram* m_program;
  QOpenGLShaderProgram* m_lighting_program;

//  QOpenGLBuffer m_vvbo;
//  QOpenGLVertexArrayObject m_vao;

  QOpenGLFramebufferObject* m_gbuffer_fbo;
  QOpenGLTexture* m_view_position_texture;

  QOpenGLVertexArrayObject* m_quad_vao;
  QOpenGLBuffer m_quad_vbo;

//  QMatrix4x4 m_model_matrix;
  QMatrix4x4 m_view_matrix;
  QMatrix4x4 m_projection_matrix;

  Camera3D m_arcCamera;
//  Transform m_myTransform;

  std::vector <std::shared_ptr <SceneObject>> m_SceneObjects;
  ShapeMap m_ShapePool;
};

void inline Scene::addSceneObject(std::string _shape){ addSceneObject(_shape, QVector3D(0.0f, 0.0f, 0.0f)); };

//-------------scene Utils------------------
void addShape();


#endif // SCENE_H
