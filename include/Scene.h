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
#include "model.h"

#include "utils.h"

class Scene : public AbstractScene
{

public:
  Scene(Window *_window);
  ~Scene();
  void initialize();
  void resize(int width, int height);
  void paint();
  void drawScreenQuad();

  void SceneInitialize();
  void QtOpenGLinitialize();
  void QtOpenGLinitialize_backup();
  void setupScene();

  void addShape(Scene *_scene, std::string _name, const QVector3D* _data, int _size);
  ModelPtr addModel(Scene *_scene, std::string _name, std::string _path);

  /*
   * WIP
  pSceneOb addSceneObject(std::string _shape);
  pSceneOb addSceneObject(std::string _shape, const QVector3D &_pos);
  pSceneOb addSceneObject(std::string _name, const QVector3D &_pos, const QQuaternion &_rot);
  */

  pSceneOb addSceneObjectFromModel(std::string _name, const uint _materialID, const QVector3D &_pos, const QQuaternion &_rot);
  LightPtr addPointLight();
  LightPtr addPointLight(const QVector3D &_pos, const QVector3D &_color);
  MaterialPtr addMaterial(const QVector3D &_ambient, const QVector3D &_diffuse, const QVector3D &_specular, float _shininess);

  ShapePtr getShapeFromPool(std::string _key);
  ModelPtr getModelFromPool(std::string _key);
  MaterialPtr getMaterial(int _index);
  pSceneOb getSceneObjectFromIndex(int _index);

private:
  friend class Window;

  QOpenGLShaderProgram* m_program;
  QOpenGLShaderProgram* m_screen_program;
  QOpenGLShaderProgram* m_lighting_program;
  QOpenGLShaderProgram* m_flat_program;

  QOpenGLFramebufferObject* m_gbuffer_fbo;
  QOpenGLFramebufferObject* m_fbo_A;
  QOpenGLFramebufferObject* m_fbo_B;

  // fbo raw OpenGL
  GLuint SCR_WIDTH, SCR_HEIGHT;
  unsigned int fbo, texture, rbo, intermediateFBO, screenTexture;

  QOpenGLTexture* m_view_position_texture;

  QOpenGLVertexArrayObject* m_quad_vao;
  QOpenGLBuffer m_quad_vbo;

  QOpenGLVertexArrayObject* m_Test_vao;
  QOpenGLBuffer m_Test_vbo;

  QMatrix4x4 m_view_matrix;
  QMatrix4x4 m_projection_matrix;

  Camera3D m_arcCamera;

  ShapeMap m_ShapePool;
  ModelMap m_ModelPool;

  std::vector <LightPtr> m_Pointlights;
  std::vector <MaterialPtr> m_Materials;
  std::vector <pSceneOb> m_SceneObjects;
};


inline MaterialPtr Scene::getMaterial(int _index){ return m_Materials[_index]; };
inline pSceneOb Scene::getSceneObjectFromIndex(int _index){ return m_SceneObjects[_index]; };
inline LightPtr Scene::addPointLight(){ return addPointLight(QVector3D(0,0,0), QVector3D(1,1,1)); };

#endif // SCENE_H
