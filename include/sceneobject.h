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
    SceneObject(Scene *_scene, std::shared_ptr<Shape> _Shape);

    void bind();
    void draw();
    void release();
    int m_Id = 33;

private:
    Transform m_Transform;
    Scene *pScene;
    //std::shared_ptr<Shape>
    std::shared_ptr<Shape> pShape;
};

#endif // SCENEOBJECT_H
