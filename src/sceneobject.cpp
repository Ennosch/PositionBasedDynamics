#include "sceneobject.h"

#include <QDebug>

SceneObject::SceneObject()
{
    qDebug()<<"dtor: initScene Object";
}
//std::shared_ptr<Shape>
SceneObject::SceneObject(Scene *_scene, std::shared_ptr<Shape> _Shape) :
    pScene(_scene),
    pShape(_Shape)
{
    qDebug()<<"ctor: initScene Object";
//    auto pShape = std::make_shared<Shape>(7);
//    //pShape->m_Id = 5;
//    qDebug()<<pShape->m_Id;

}

void SceneObject::bind()
{
    pShape->m_vao.bind();
}

void SceneObject::draw()
{

}

void SceneObject::release()
{
    pShape->m_vao.release();
    pShape->m_vvbo.release();
}

