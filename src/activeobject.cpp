#include "activeobject.h"

#include "GLWidget.h"


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

    // pick dynamicSceneObject Case:


    activeSceneObject = _sender;

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
    if(activeSceneObject)
    {
//        mlog<<"hello";
        if(activeSceneObject->isDynamic())
        {
            activeSceneObject->isDynamic(false);
//            activeSceneObject->dynamicObject()->pinToPosition(activeSceneObject->getPos());
            m_pinConstraint = std::make_shared<PinConstraint>(activeSceneObject->dynamicObject()->particle(), activeSceneObject->getPos());
            activeSceneObject->dynamicObject()->particle()->m_Constraints.push_back(m_pinConstraint);
            m_pickedDynamic = true;
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
            m_pinConstraint->setPositon(activeSceneObject->getPos());
            m_pickedDynamic = true;
        }
    }
}

void ActiveObject::onReleased()
{
    if(activeSceneObject)
    {
        if(m_pickedDynamic)
        {
//            activeSceneObject->dynamicObject()->endPinToPosition();
//            activeSceneObject->dynamicObject()->particle()->m_Constraints.
//            activeSceneObject->isDynamic(true);
        }
    }
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
