#include "activeobject.h"

#include "GLWidget.h"

#include "Scene.h"
#include "dynamics/dynamicsWorld.h"
#include "dynamics/particle.h"


ActiveObject::ActiveObject()
{

}

ActiveObject::ActiveObject(GLWidget *_widget)
{
    m_GLWidget = _widget;
//    connect(this, SIGNAL(transformChanged(int a)), );
}

void ActiveObject::notify(int _id)
{
    // set
    qDebug()<<_id<<"notified me";
}

void ActiveObject::notify(pSceneOb _sender)
{
    if(_sender == nullptr)
    {
        m_isActive = false;
        m_manipulator->setActive(false);

        activeSceneObject = nullptr;
        return;
    }

    if(_sender != activeSceneObject)
    {
        m_pickedDynamic = _sender->isDynamic();
    }
    // pick dynamicSceneObject Case:
    // WIP check rigidBody particle initialization, something is wrong there.
    activeSceneObject = _sender;
    m_isActive = true;

    m_manipulator->setActive(true);

    emit transformChanged(_sender->getMatrix(),
                          _sender->getPos(),
                          _sender->getPos(),
                          _sender->getPos());
    m_manipulator->setTranslation(_sender->getTransform().translation());
    m_manipulator->setRotation(_sender->getTransform().rotation());
}

void ActiveObject::notify(const Transform &_t)
{
    if(activeSceneObject)
    {
//        if(activeSceneObject->isDynamic())
//            activeSceneObject->pDynamicObject->setTransform(_t.toMatrix(0));
        activeSceneObject->setRotation(_t.rotation());
        activeSceneObject->setTranslation(_t.translation());
    }
    //feed back Transform into dynamicObject
}

void ActiveObject::notify(MouseState _mouseState)
{

}

void ActiveObject::onClicked()
{
    auto dw = m_GLWidget->scene()->dynamicsWorld();
    if(activeSceneObject)
    {
        if(activeSceneObject->isDynamic())
        {
            activeSceneObject->isDynamic(false);
            Particle *ptr = nullptr;
            auto particleSmartPointer = activeSceneObject->dynamicObject()->pointer(ptr);
            m_pinConstraint = std::make_shared<PinConstraint>(particleSmartPointer, activeSceneObject->getPos());
            // Pushing back shared_ptr. Expecting conversion to weak_ptr.
            activeSceneObject->dynamicObject()->m_Constraints.push_back(m_pinConstraint);

            m_pickedDynamic = true;
        }
        else{
            mlog<<"--------Missing Constraint for active object";
        }
    }
}

void ActiveObject::onPressed()
{
    if(activeSceneObject)
    {
        {
//            activeSceneObject->dynamicObject()->endPinToPosition();
//            activeSceneObject->dynamicObject()->pinToPosition(activeSceneObject->getPos());
            if(m_pinConstraint)
            {
                m_pinConstraint->setPositon(activeSceneObject->getPos());
//                m_pickedDynamic = true;
            }
        }
    }
}

void ActiveObject::onReleased()
{
    if(activeSceneObject)
    {
        if(m_pickedDynamic)
        {
//            mlog<<" onReleased 1"<<m_pickedDynamic<<"           "<<activeSceneObject.get();
            // (Improve) dont include scene and dynamicsWorld. Just ask constraint to delete itself ?
            auto dw = m_GLWidget->scene()->dynamicsWorld();
            dw->deleteConstraint(m_pinConstraint);
            activeSceneObject->isDynamic(true);

            m_pinConstraint.reset();
            mlog<<" onReleased 2";
        }
        m_pickedDynamic = false;
        mlog<<" onReleased 3";
    }
    activeSceneObject = nullptr;
    mlog<<" onReleased 4";
}

pSceneOb ActiveObject::currentObject()
{
    return activeSceneObject;
}

bool ActiveObject::isActive()
{
    return m_isActive;
}

void ActiveObject::setTransform(const QVector3D _t, const QVector3D _r, const QVector3D _s)
{
//    qDebug()<<"set Transform ";
    if(activeSceneObject)
    {
       activeSceneObject->setTranslation(_t);
       activeSceneObject->setRotation(_r);
//       activeSceneObject->setScale(_s);
    }
}
