#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <memory>

#include <QMatrix4x4>

#include "transform.h"
#include "shape.h"
#include "model.h"
#include "utils.h"
#include "dynamics/dynamicObject.h"

class Scene;

class Model;

class SceneObject
{
public:
    SceneObject();
    SceneObject(Scene *_scene, ShapePtr _Shape);
    SceneObject(Scene *_scene, ShapePtr _Shape, const QVector3D &_pos);
    SceneObject(Scene *_scene, ShapePtr _Shape, const QVector3D &_pos, const QQuaternion &_rot);
    SceneObject(Scene *_scene, ModelPtr _Model, const uint _materialID, const QVector3D &_pos, const QQuaternion &_rot);
    SceneObject(std::string _path);

    void bind();
    void draw();
    void drawOld();
    void release();
    void setTranslation(const QVector3D &_dt);
    void translate(const QVector3D &_dt);
    void setScale(const QVector3D &_s);
    void rotate(const QQuaternion &_rot);
    void makeDynamic(DynamicObjectPtr _dynamicObject);
    bool isDynamic();

    const QMatrix4x4 getMatrix();
    const QVector3D getPos();
    const Transform getTransform();
    uint getMaterialID();
    ShapePtr shape();

private:
    bool m_IsDynamic = false;
    uint m_MaterialID;    
    ShapePtr pShape;
    ModelPtr pModel;
    MaterialPtr pMaterial;
    Transform m_Transform;
    Scene *pScene;
    DynamicObjectPtr pDynamicObject = nullptr;

};

inline uint SceneObject::getMaterialID(){ return m_MaterialID;};
inline const QVector3D SceneObject::getPos(){ return m_Transform.translation(); };
inline std::shared_ptr<Shape> SceneObject::shape(){return pShape; };

inline bool SceneObject::isDynamic(){ return m_IsDynamic; };

inline void SceneObject::setTranslation(const QVector3D &_dt){m_Transform.setTranslation(_dt); };
inline void SceneObject::translate(const QVector3D &_dt){m_Transform.translate(_dt); };
inline void SceneObject::rotate(const QQuaternion &_rot){m_Transform.rotate(_rot); };
inline void SceneObject::setScale(const QVector3D &_s){m_Transform.scale(_s); };
inline const Transform SceneObject::getTransform(){return m_Transform;};

inline void SceneObject::drawOld(){ pShape->drawOld(); };

#endif // SCENEOBJECT_H
