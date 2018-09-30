#ifndef DYNAMICSWORLD_H
#define DYNAMICSWORLD_H

#include <vector>
#include <memory>

#include <QDebug>
#include <QVector3D>

#include "transform.h"
#include "utils.h"
#include "sceneobject.h"
#include "hashgrid.h"
#include "dynamics/dynamicObject.h"
#include "dynamics/particle.h"


class DynamicsWorld
{
    public:
        DynamicsWorld();
        void initialize();
        void update();

        void addDynamicObject(pSceneOb _sceneObject);
        ParticlePtr addParticle(float _x, float _y, float _z);

        void foo(){qDebug("Dynamics World Foo");}
        int pCount = 0;

    private:
        int m_ID = 0;
        std::vector <DynamicObjectPtr> m_DynamicObjects;
        std::vector <ParticlePtr> m_Particles;
        HashGrid m_hashGrid;

};

#endif // DYNAMICSWORLD_H
