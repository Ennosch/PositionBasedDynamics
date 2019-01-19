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
#include "GLWidget.h"
#include "transform.h"
#include "camera.h"
#include "sceneobject.h"
#include "shape.h"
#include "model.h"
#include "utils.h"
#include "dynamics/dynamicsWorld.h"
#include "dynamics/collisiondetection.h"

class Scene : public AbstractScene
{
  static int numCreation;

public:
  Scene(Window *_window);
  Scene(GLWidget *_widget);
  ~Scene();
  void initialize();
  void resize(int width, int height);
  void paint();
  void updateSceneObjects();
  void drawScreenQuad();

  void setDynamicsWorld(DynamicsWorld *_world);
  void QtOpenGLinitialize();
  void DynamicsInitialize();
  void setupScene();

  // creation
  void addShape(Scene *_scene, std::string _name, const QVector3D* _data, int _size);
  ModelPtr addModel(Scene *_scene, std::string _name, std::string _path);

  pSceneOb addSceneObjectFromModel(std::string _name, const uint _materialID, const QVector3D &_pos, const QQuaternion &_rot);
  LightPtr addPointLight();
  LightPtr addPointLight(const QVector3D &_pos, const QVector3D &_color);
  MaterialPtr addMaterial(const QVector3D &_ambient, const QVector3D &_diffuse, const QVector3D &_specular, float _shininess);
  void addLine(const Line &_line);
  void addLine(const Ray &_ray, float t);
  void addLine(const QVector3D &_start, const QVector3D &_end);
  void makeDynamic(pSceneOb _sceneObject);

  // accessor
  ShapePtr getShapeFromPool(std::string _key);
  ModelPtr getModelFromPool(std::string _key);
  MaterialPtr getMaterial(int _index);
  pSceneOb getSceneObjectFromIndex(int _index);
  pSceneOb getPointerFromSceneObject(const SceneObject *_sceneObject);

  QVector3D *getData();
  DynamicsWorld* dynamicsWorld();

  Ray castRayFromCamera(float ndcX, float ndcY, float depthZ);
  pSceneOb pickObject(float ndcX, float ndcY);

  void updateLinesVBO();

  CollisionDetection m_CollisionDetect;
  QVector3D currentRayStart = QVector3D(0,0,0);
  QVector3D currentRayEnd = QVector3D(0,0,0);

private:
  friend class Window;
  friend class GLWidget;

  DynamicsWorld *m_DynamicsWorld;

  QOpenGLShaderProgram* m_program;
  QOpenGLShaderProgram* m_screen_program;
  QOpenGLShaderProgram* m_lighting_program;
  QOpenGLShaderProgram* m_flat_program;

  //  QOpenGLFramebufferObject* m_gbuffer_fbo;
  // fbo raw OpenGL

  int SCR_HEIGHT, SCR_WIDTH;
  unsigned int fbo, texture, rbo, intermediateFBO, screenTexture;

  QOpenGLTexture* m_view_position_texture;
  QOpenGLVertexArrayObject* m_quad_vao;
  QOpenGLBuffer m_quad_vbo;

  QOpenGLVertexArrayObject* m_lines_vao;
  QOpenGLBuffer m_lines_vbo;

  QMatrix4x4 m_projection_matrix;
  Camera3D m_arcCamera;

  ShapeMap m_ShapePool;
  ModelMap m_ModelPool;

  std::vector <LightPtr> m_Pointlights;
  std::vector <MaterialPtr> m_Materials;
  std::vector <pSceneOb> m_SceneObjects;
  std::vector <Line> m_Lines;

  pSceneOb m_pickedObject;
};

inline MaterialPtr Scene::getMaterial(int _index){ return m_Materials[_index]; };
inline pSceneOb Scene::getSceneObjectFromIndex(int _index){ return m_SceneObjects[_index]; };
inline LightPtr Scene::addPointLight(){ return addPointLight(QVector3D(0,0,0), QVector3D(1,1,1)); };

/*
 * WIP
pSceneOb addSceneObject(std::string _shape);
pSceneOb addSceneObject(std::string _shape, const QVector3D &_pos);
pSceneOb addSceneObject(std::string _name, const QVector3D &_pos, const QQuaternion &_rot);
*/

#endif // SCENE_H
