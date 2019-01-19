#ifndef DYNAMICSWORLDCONTROLLER_H
#define DYNAMICSWORLDCONTROLLER_H

#include <QObject>

class Scene;

class DynamicsWorld;

class DynamicsWorldController
{
public:
    DynamicsWorldController();

private:
    DynamicsWorld *m_dynamicsWorld;
    Scene *m_scene;
};

#endif // DYNAMICSWORLDCONTROLLER_H
