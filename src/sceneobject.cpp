#include "sceneobject.h"

#include <QDebug>

SceneObject::SceneObject()
{
    qDebug()<<"dtor: initScene Object";
}

SceneObject::SceneObject(Scene *_scene)
{
    qDebug()<<"ctor: initScene Object";
//    auto pShape = std::make_shared<Shape>(7);
//    //pShape->m_Id = 5;
//    qDebug()<<pShape->m_Id;

}

