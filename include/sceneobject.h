#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <memory>

#include "transform.h"
#include "shape.h"

class Scene;

class SceneObject
{
public:
    SceneObject();
    SceneObject(Scene *_scene);
private:
    Transform m_Transform;
    Scene *pScene;
    std::shared_ptr<Shape> *pShape;
};

#endif // SCENEOBJECT_H
