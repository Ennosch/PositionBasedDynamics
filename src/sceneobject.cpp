#include "sceneobject.h"

#include <QDebug>

SceneObject::SceneObject()
{
    qDebug()<<"dtor: initScene Object";
}

SceneObject::SceneObject(Scene *_scene, ShapePtr _Shape) :
    pScene(_scene),
    pShape(_Shape)
{
    qDebug()<<"ctor: initScene Object";
}

SceneObject::SceneObject(Scene *_scene, ShapePtr _Shape,
                         const QVector3D &_pos) :
    pScene(_scene),
    pShape(_Shape)
{
    m_Transform.setTranslation(_pos);
}

SceneObject::SceneObject(Scene *_scene, ShapePtr _Shape,
                         const QVector3D &_pos,
                         const QQuaternion &_rot) :
    pScene(_scene),
    pShape(_Shape)
{

    m_Transform.setTranslation(_pos);
    m_Transform.rotate(_rot);
}

SceneObject::SceneObject(Scene *_scene, ModelPtr _Model,
                         const QVector3D &_pos,
                         const QQuaternion &_rot) :
    pScene(_scene),
    pModel(_Model)
{
    m_Transform.setTranslation(_pos);
    m_Transform.rotate(_rot);
}

SceneObject::SceneObject(std::string _path)
{

}

void SceneObject::bind()
{
    pShape->bind();
}

void SceneObject::draw()
{
//    pShape->draw();
    if(pModel != NULL)
    {
        pModel->draw();
    }
    else
    {
        pShape->drawOld();
    }
}

void SceneObject::release()
{
    pShape->release();
}
