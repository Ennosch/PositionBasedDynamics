#include "sceneobject.h"

#include <QDebug>

SceneObject::SceneObject()
{
    qDebug()<<"dtor: initScene Object";
}

SceneObject::SceneObject(Scene *_scene, std::shared_ptr<Shape> _Shape) :
    pScene(_scene),
    pShape(_Shape)
{
    qDebug()<<"ctor: initScene Object";
}

SceneObject::SceneObject(Scene *_scene, std::shared_ptr<Shape> _Shape, const QVector3D &_pos) :
    pScene(_scene),
    pShape(_Shape)
{
    m_Transform.setTranslation(_pos);
}


void SceneObject::bind()
{
    pShape->bind();
}

void SceneObject::draw()
{

}

void SceneObject::release()
{
    pShape->release();
}
