#ifndef DYNAMICSWORLD_H
#define DYNAMICSWORLD_H

#include <vector>
#include <memory>

#include <QDebug>
#include <QVector3D>
//#include <eigen3/Eigen/Dense>

#include "transform.h"
#include "utils.h"
#include "sceneobject.h"
#include "hashgrid.h"
#include "dynamics/dynamicObject.h"
#include "dynamics/particle.h"
#include "dynamics/collisiondetection.h"
#include "dynamics/constraint.h"

typedef QVector3D Vec3;

class DynamicsWorld
{
    public:
        DynamicsWorld();
        void initialize();
        void update(float dt = 0.5);
        void info();

        void addDynamicObject(pSceneOb _sceneObject);
        DynamicObjectPtr addDynamicObjectAsParticle(pSceneOb _sceneObject, ParticlePtr _particle);

        ParticlePtr addParticle(float _x, float _y, float _z);
        void addPlane(const Plane &_plane);

        void checkSphereSphere(ParticlePtr p1, ParticlePtr p2);
        void checkSpherePlane(ParticlePtr p1, const Plane &_plane);
        void checkSpherePlaneOld(ParticlePtr p1, const Plane &_plane);

        void generateData();

        int pCount = 0;
        int objectCount = 0;

//    private:
        int m_ID = 0;
        std::vector <DynamicObjectPtr>  m_DynamicObjects;
        std::vector <ParticlePtr>       m_Particles;
        std::vector <Plane>             m_Planes;

        HashGrid m_hashGrid;
        CollisionDetection m_CollisionDetect;

};

#endif // DYNAMICSWORLD_H
