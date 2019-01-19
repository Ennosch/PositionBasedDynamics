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
//    qDebug()<<"getting notified: "<<_sender.use_count();
    activeSceneObject = _sender;

    emit transformChanged(_sender->getMatrix(),
                          _sender->getPos(),
                          _sender->getPos(),
                          _sender->getPos());
}

void ActiveObject::setTransform(const QVector3D _t, const QVector3D _r, const QVector3D _s)
{
//    qDebug()<<"set Transform ";
    if(activeSceneObject)
    {
       activeSceneObject->setTranslation(_t);
    }
}
