#include "dynamics/dynamicsWorldController.h"

#include "dynamics/dynamicsWorld.h"

DynamicsWorldController::DynamicsWorldController()
{

}

DynamicsWorldController::DynamicsWorldController(DynamicsWorld *_dynamics)
{
    m_dynamicsWorld = _dynamics;
}

void DynamicsWorldController::startStopSim()
{
    if(m_simulating)
    {
        m_dynamicsWorld->setSimulate(false);
        m_simulating = false;
    }
    else
    {
        m_dynamicsWorld->setSimulate(true);
        m_simulating = true;
    }
}

void DynamicsWorldController::stepSim()
{
    m_dynamicsWorld->step();
}



void DynamicsWorldController::resetSim()
{

}

void DynamicsWorldController::setGravityY(float _y)
{
    mlog<<"set gravity"<<_y;
    m_dynamicsWorld->m_gravity.setY(_y);
}

void DynamicsWorldController::setTimeStepSize(float _ts)
{
    mlog<<"set time step size"<<_ts;
       m_dynamicsWorld->m_dt = _ts;
}

void DynamicsWorldController::setPreConditionIteration(int _pciter)
{
    m_dynamicsWorld->m_preConditionIteration = _pciter;
}

void DynamicsWorldController::setConstraintIteration(int _citer)
{
    mlog<<"set constraint iter"<<_citer;
    m_dynamicsWorld->m_constraintIteration = _citer;
}

void DynamicsWorldController::setPBDDamping(float _damp)
{
    mlog<<"set damp iter"<<_damp;
    m_dynamicsWorld->m_pbdDamping = _damp;
}

void DynamicsWorldController::setDistanceConstraintStretch(float _stretch)
{
    mlog<<"setting stretch "<<_stretch;
    m_dynamicsWorld->m_DistanceConstraintStretch = _stretch;
}

void DynamicsWorldController::setDistanceConstraintCompress(float _compress)
{
    mlog<<"setting compress "<<_compress;
    m_dynamicsWorld->m_distanceConstraintCompress = _compress;
}

void DynamicsWorldController::setShapeMatchingConstraintAttract(float _attract)
{
    mlog<<"setting _attract "<<_attract;
    m_dynamicsWorld->m_shapeMatchAttract = _attract;
}

void DynamicsWorldController::setParticleMass(float _mass)
{
    m_dynamicsWorld->setAllParticlesMass(_mass);
}
