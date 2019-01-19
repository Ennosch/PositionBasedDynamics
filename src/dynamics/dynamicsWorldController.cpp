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

void DynamicsWorldController::setStepSize()
{

}

void DynamicsWorldController::resetSim()
{

}
