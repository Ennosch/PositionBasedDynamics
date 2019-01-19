#include "sceneobject.h"

#include <QDebug>

#include "Scene.h"
#include "activeobject.h"

SceneObject::SceneObject()
{
    qDebug()<<"dtor: initScene Object";
}

SceneObject::SceneObject(Scene *_scene, ShapePtr _Shape) :
    pScene(_scene),
    pShape(_Shape)
{
//   auto Test = _scene->foo();

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

SceneObject::SceneObject(Scene *_scene,
                         ModelPtr _Model,
                         const uint _materialID,
                         const QVector3D &_pos,
                         const QQuaternion &_rot) :
    pScene(_scene),
    pModel(_Model),
    m_MaterialID(_materialID)
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
    if(pModel != nullptr)
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

void SceneObject::update()
{
    if(isDynamic())
    {
        m_ModelMatrix = pDynamicObject->getTransfrom();
        setTranslation(pDynamicObject->getTranslation());

        if(m_IsActive)
            notify();
    }
    else if(m_IsDirty)
    {
        m_IsDirty = false;
        m_ModelMatrix =  m_Transform.toMatrix();

        if(m_IsActive)
            notify();
    }
}

void SceneObject::isActive(bool _isActive)
{
    m_IsActive = _isActive;
}

bool SceneObject::isActive()
{
    return m_IsActive;
}

void SceneObject::makeDynamic(DynamicObjectPtr _dynamicObject)
{
    pDynamicObject = _dynamicObject;
    m_IsDynamic = true;
}

void SceneObject::setActiveObject(ActiveObject *_activeObject)
{
    activeObject = _activeObject;
}

void SceneObject::setID(uint _id)
{
    m_ID = _id;
}

void SceneObject::notify()
{
    pSceneOb pSO = pScene->getPointerFromSceneObject(this);
//    qDebug()<<"instances: "<<pSO.use_count();


    activeObject->notify(pSO);

//    qDebug()<<"instances2: "<<pSO.use_count();
}

const QMatrix4x4 SceneObject::getMatrix()
{
    return m_ModelMatrix;
}

uint SceneObject::getID()
{
    return  m_ID;
}















