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
#include "dynamicsWorldController.h"

typedef QVector3D Vec3;

class DynamicsWorld
{
    public:
        DynamicsWorld();
        void initialize();
        void update();
        void info();
        DynamicsWorldController* controller();
        void setSimulate(bool _isSimulating);
        void reset();
        void step();

        void addDynamicObject(pSceneOb _sceneObject);
        DynamicObjectPtr addDynamicObjectAsParticle(pSceneOb _sceneObject, ParticlePtr _particle);

        ParticlePtr addParticle(float _x, float _y, float _z);
        void addPlane(const Plane &_plane);

        void checkSphereSphere(ParticlePtr p1, ParticlePtr p2);
        void checkSpherePlane(ParticlePtr p1, const Plane &_plane);

        void generateData();

        QVector3D* debugDrawLineData();

        void foo(){ qDebug("foo"); }

        int pCount = 0;
        int objectCount = 0;

//    private:
        bool m_simulate;
        int m_ID = 0;
        float m_dt;
        DynamicsWorldController         *m_DynamicsWorldController;
        std::vector <DynamicObjectPtr>  m_DynamicObjects;
        std::vector <ParticlePtr>       m_Particles;
        std::vector <Plane>             m_Planes;

        std::vector<QVector3D*>         m_debugLines;

        HashGrid m_hashGrid;
        CollisionDetection m_CollisionDetect;

};

#endif // DYNAMICSWORLD_H
