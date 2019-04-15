#ifndef DYNAMICSWORLDCONTROLLER_H
#define DYNAMICSWORLDCONTROLLER_H

#include <QObject>

class Scene;

class DynamicsWorld;

class DynamicsWorldController : public QObject
{
Q_OBJECT
public:
    DynamicsWorldController();
    DynamicsWorldController(DynamicsWorld* _dynamics);

protected slots:
    void startStopSim();
    void stepSim();
    void setStepSize();
    void resetSim();


private:
    bool m_simulating;
    DynamicsWorld *m_dynamicsWorld;

};

#endif // DYNAMICSWORLDCONTROLLER_H
