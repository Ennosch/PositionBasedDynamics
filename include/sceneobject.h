#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <memory>

#include <QMatrix4x4>

#include "transform.h"
#include "shape.h"

class Scene;

class SceneObject
{
public:
    SceneObject();
    SceneObject(Scene *_scene, std::shared_ptr<Shape> _Shape);

    void bind();
    void draw();
    void release();
    void setTranslation(const QVector3D &_dt);
    const QMatrix4x4 getMatrix();
    int m_Id = 33;

private:
    Transform m_Transform;
    Scene *pScene;
    //std::shared_ptr<Shape>
    std::shared_ptr<Shape> pShape;
};

inline const QMatrix4x4 SceneObject::getMatrix(){return m_Transform.toMatrix();};
inline void SceneObject::setTranslation(const QVector3D &_dt){m_Transform.translate(_dt);};

#endif // SCENEOBJECT_H
