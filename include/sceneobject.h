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
    SceneObject(Scene *_scene, std::shared_ptr<Shape> _Shape, const QVector3D &_pos);
    SceneObject(Scene *_scene, std::shared_ptr<Shape> _Shape, const QVector3D &_pos, const QQuaternion &_rot);
    SceneObject(std::string _path);

    void bind();
    void draw();
    void release();
    void setTranslation(const QVector3D &_dt);
    void translate(const QVector3D &_dt);
    void setScale(const QVector3D &_s);
    void rotate(const QQuaternion &_rot);
    const QMatrix4x4 getMatrix();
    const QVector3D getPos();

    std::shared_ptr<Shape> shape();
    int m_Id = 33;

private:

    Scene *pScene;
    //std::shared_ptr<Shape>
    std::shared_ptr<Shape> pShape;
    Transform m_Transform;
};

inline const QMatrix4x4 SceneObject::getMatrix(){return m_Transform.toMatrix();};
inline const QVector3D SceneObject::getPos(){ return m_Transform.translation(); };
inline std::shared_ptr<Shape> SceneObject::shape(){return pShape; };

inline void SceneObject::setTranslation(const QVector3D &_dt){m_Transform.setTranslation(_dt); };
inline void SceneObject::translate(const QVector3D &_dt){m_Transform.translate(_dt); };
inline void SceneObject::rotate(const QQuaternion &_rot){m_Transform.rotate(_rot); };
inline void SceneObject::setScale(const QVector3D &_s){m_Transform.scale(_s); };


#endif // SCENEOBJECT_H
