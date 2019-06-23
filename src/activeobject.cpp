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

    // pick dynamicSceneObject Case:
    // WIP check rigidBody particle initialization, something is wrong there.
    activeSceneObject = _sender;

    m_isActive = true;

    m_manipulator->setActive(true);

    int var = 0;
    if(activeSceneObject->isDynamic())
    {
        var = activeSceneObject->dynamicObject()->mID;
    }

    emit transformChanged(_sender->getMatrix(),
                          _sender->getPos(),
                          _sender->getPos(),
                          _sender->getPos(),
                          var);
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

void ActiveObject::pinConstraintActive()
{
    Particle *ptr = nullptr;
    auto particleSmartPointer = activeSceneObject->dynamicObject()->pointer(ptr);
    auto pinConstraint = std::make_shared<PinConstraint>(particleSmartPointer, activeSceneObject->getPos());
    activeSceneObject->setPinConstraint(pinConstraint);
    particleSmartPointer->m_Constraints.push_back(pinConstraint);
}

void ActiveObject::updatePinConstraintActive()
{
    if(!activeSceneObject)
        return;
//    if(m_pinConstraint)
    {
         auto pinConstraint  = activeSceneObject->getPinConstraint();
         if(!pinConstraint)
             mlog<<"no Cstr-----------";
         pinConstraint->setPositon(activeSceneObject->getPos());
    }
}

void ActiveObject::unpinConstraintActive()
{
    if(!activeSceneObject)
        return;
    auto dw = m_GLWidget->scene()->dynamicsWorld();
    dw->deleteConstraint(activeSceneObject->getPinConstraint());
}

void ActiveObject::processInput(ActiveObject::AOInput _input)
{
    // picking sets SELECTED. Make funciton select that sets selected or pinned.
    switch(_input)
    {
        case LM_CLICKED:
//                if(inputManager::keyPressed(Qt::Key_Shift))
                if(inputManager::keyPressed(Qt::Key_Shift) && activeSceneObject->isDynamic()){
                    Particle* empty = nullptr;
                    ParticlePtr myP = activeSceneObject->dynamicObject()->pointer(empty);
                    addParticleToSelection(myP);
                }

                if(m_state == SELECTED && activeSceneObject->isDynamic()){
                    if(inputManager::keyPressed(Qt::Key_Shift)){
//                        addParticleToSelection(activeSceneObject->dynamicObject()->pointer());
                        Particle* empty = nullptr;
                        ParticlePtr myP = activeSceneObject->dynamicObject()->pointer(empty);
                        addParticleToSelection(myP);

                    }
                    pinConstraintActive();
//                    if(activeSceneObject)
                    bool was = activeSceneObject->isHidden();
                    activeSceneObject->isPinned(true);
                    activeSceneObject->isHidden(false);
                    activeSceneObject->isDynamic(false);
                    m_state = PICKED;

//                    mlog<<" SELECTED -> PICKED isHidden: "<<activeSceneObject->isHidden()<< " was: "<<was;
                }

                if(m_state == PINNED)
                    activeSceneObject->isDynamic(false);
            break;

        case LM_PRESSED:
                if(m_state == PINNED)
                    updatePinConstraintActive();
                if(m_state == PICKED)
                    updatePinConstraintActive();
            break;

        case LM_RELEASED:
                 if(m_state == PINNED){
                     bool was = activeSceneObject->isHidden();
                     activeSceneObject->isDynamic(true);
                     activeSceneObject->isPinned(true);
                     activeSceneObject->isHidden(false);
//                    mlog<<" set pined isHidden: "<<activeSceneObject->isHidden()<< " was: "<<was;
                 }

                if(m_state == PICKED){
                    unpinConstraintActive();
                    bool was = activeSceneObject->isHidden();
                    activeSceneObject->isPinned(false);
                    activeSceneObject->isHidden(true);
                    activeSceneObject->isDynamic(true);
                    m_state = SELECTED;
//                    mlog<<" set unppined isHidden: "<<activeSceneObject->isHidden()<< " was: "<<was;
                }
            break;

        case B_P_PRESSED:
//                mlog<<"P pressed"<<m_state;

                if(m_state == PINNED){
                    unpinConstraintActive();
                    bool was = activeSceneObject->isHidden();
                    activeSceneObject->isPinned(false);
                    activeSceneObject->isHidden(true);
                    activeSceneObject->isDynamic(true);
                    m_state = SELECTED;
//                    mlog<<" PINNED -> SELECTED: "<<activeSceneObject->isHidden()<< " was: "<<was;
                    break;
                }

                if(m_state == PICKED){
                    updatePinConstraintActive();
                    m_state = PINNED;
                    break;
                }

                if(m_state == SELECTED){
                    pinConstraintActive();
                    activeSceneObject->isDynamic(true);
                    m_state = PINNED;
                }
            break;

        case B_T_PRESSED:
                addPinTogetherConstraintToSelection();
                break;

        case B_D_PRESSED:
                mlog<<"D pressed";
                deletePinTogetherConstraintFromSelection();
                break;
    }
}

void ActiveObject::addParticleToSelection(const ParticlePtr _p)
{
   if( std::find(m_selection.begin(),m_selection.end(), _p) == m_selection.end())
   {
       m_selection.push_back(_p);
       mlog<<"added particle to selection "<<m_selection.size();
    }
   else{

   }
}

void ActiveObject::addPinTogetherConstraintToSelection()
{
//    std::shared_ptr<PinTogetherConstraint> ptCstr = std::make_shared<PinTogetherConstraint>(m_selection);
    m_GLWidget->scene()->dynamicsWorld()->addPinTogetherConstraint(m_selection);
}

void ActiveObject::deletePinTogetherConstraintFromSelection()
{
    mlog<<"D pressed fuync";
    if(m_state == SELECTED && activeSceneObject->isDynamic()){
        Particle *ptr = nullptr;
        ParticlePtr p = activeSceneObject->dynamicObject()->pointer(ptr);
        int i = 0;
        mlog<<"PRe loop";
        for(auto c : p->m_Constraints)
        {
            i++;
//            mlog<<" constraint"<<i<<" type:"<<c.lock()->type()<<" "<<AbstractConstraint::PINTOGETHER;
            if(c.lock()->type() == AbstractConstraint::PINTOGETHER)
            {
                mlog<<" constraint"<<i<<" type:"<<c.lock()->type()<<" is PINT CONSTR delete";
                m_GLWidget->scene()->dynamicsWorld()->deleteConstraint(c.lock());
            }

        }
    }
    mlog<<"LOG4";
}

pSceneOb ActiveObject::currentObject()
{
    return activeSceneObject;
}

bool ActiveObject::isActive()
{
    return m_isActive;
}

void ActiveObject::select()
{
//    if(m_state == NONE_SELECTED )

    if(activeSceneObject->isPinned()){
        m_state = PINNED;
        return;
    }

    m_state = SELECTED;
}

void ActiveObject::deselect()
{
    m_state = NONE_SELECTED;
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

void ActiveObject::setManipulator(Manipulator *_manipulator)
{
    m_manipulator = _manipulator;
}

void ActiveObject::setState(ActiveObject::AOState _state)
{
    m_state = _state;
}


