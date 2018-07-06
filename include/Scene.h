
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
typedef std::shared_ptr <SceneObject> pSceneOb;

struct Light
{
    QVector3D position,
        direction,
        ambient,
        diffuse,
        specular;
};

struct BufferSpec
{
    int index;
    int attrOffset;
    int sizeTuple;
    int stride;
};

class Scene : public AbstractScene
{


public:
  Scene(Window *_window);
  ~Scene();
  void initialize();
  void resize(int width, int height);
  void paint();
  void update();

  void SceneInitialize();
  void QtOpenGLinitialize();
  void initLights();

  static void addShape(Scene *_scene, std::string _name, const QVector3D* _data, int _size);
  pSceneOb addSceneObject(std::string _shape);
  pSceneOb addSceneObject(std::string _shape, const QVector3D &_pos);
  pSceneOb addSceneObject(std::string _shape, const QVector3D &_pos, const QQuaternion &_rot);

  float randf(int _mod = 10);
  QVector3D randVec3(int _mod = 10);
  float randfinRange(int a, int b);

  std::shared_ptr<Shape> getShapeFromPool(std::string _key);
  pSceneOb getShapeFromIndex(int _index);


private:
  friend class Window;

  // to cache location the uniform vars

  int m_Id = 5;
  static int m_ShapeCount;
  static int m_SceneObjectCount;


  QOpenGLShaderProgram* m_program;
  QOpenGLShaderProgram* m_screen_program;
  QOpenGLShaderProgram* m_lighting_program;
  QOpenGLShaderProgram* m_flat_program;

  QOpenGLFramebufferObject* m_gbuffer_fbo;
  QOpenGLTexture* m_view_position_texture;

  QOpenGLVertexArrayObject* m_quad_vao;
  QOpenGLBuffer m_quad_vbo;

  QMatrix4x4 m_view_matrix;
  QMatrix4x4 m_projection_matrix;

  Camera3D m_arcCamera;
  std::vector <Light> m_lights;

  std::vector <pSceneOb> m_SceneObjects;
  ShapeMap m_ShapePool;

  //-----tmp and Wip------
//  QMatrix4x4 m_model_matrix;
//  QOpenGLBuffer m_vvbo;
//  QOpenGLVertexArrayObject m_vao;
  QVector3D m_lightPos = QVector3D(0.0f, 5.0f, 0.0f);



};



//-------------scene Utils------------------
void addShape();

inline pSceneOb Scene::getShapeFromIndex(int _index){ return m_SceneObjects[_index]; };

#endif // SCENE_H
