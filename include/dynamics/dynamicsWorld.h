#ifndef DYNAMICSWORLD_H
#define DYNAMICSWORLD_H

#include <vector>
#include <memory>

#include <QDebug>
#include <QVector3D>

//#include <eigen3/Eigen/Dense>

#include "transform.h"
#include "utils.h"
#include "dynamics/dynamicUtils.h"
#include "sceneobject.h"
#include "hashgrid.h"
#include "dynamics/dynamicObject.h"
#include "dynamics/particle.h"
#include "dynamics/rigidBody.h"
#include "dynamics/softBody.h"
#include "dynamics/collisiondetection.h"
#include "dynamics/constraint.h"
#include "dynamicsWorldController.h"

typedef QVector3D Vec3;

class Scene;

class DynamicsWorld
{
    public:
        DynamicsWorld();
        void initialize();
        void initialize(Scene *_scene);
        void update();
        void info();
        DynamicsWorldController* controller();
        void setSimulate(bool _isSimulating);
        void reset();
        void step();

        void addDynamicObject(pSceneOb _sceneObject);
        DynamicObjectPtr addDynamicObjectAsParticle(pSceneOb _sceneObject);
        DynamicObjectPtr addDynamicObjectAsRigidBody(pSceneOb _sceneObject);
        DynamicObjectPtr addDynamicObjectAsSoftBody(pSceneOb _sceneObject);

        ParticlePtr getParticlePtrFromRawPtr (Particle *_ptr);
        ParticlePtr addParticle(float _x, float _y, float _z);
        void addPlane(const Plane &_plane);

        void checkSphereSphere(const ParticlePtr p1, const ParticlePtr p2);
        void checkSpherePlane(const ParticlePtr p1, const Plane &_plane);

        std::shared_ptr<DistanceEqualityConstraint> addDistanceEqualityConstraint(const ParticlePtr _p1, const ParticlePtr _p2);
        void addParticleParticleConstraint(const ParticlePtr _p1, const ParticlePtr _p2);
        void deleteConstraint(const ConstraintPtr _constraint);
        void deleteParticle();

        void generateData();
        int frameCount();

        QVector3D* debugDrawLineData();

        int pCount = 0;
        int objectCount = 0;

//    private:
        bool m_simulate;
        int m_ID = 0;
        int m_frameCount;
        float m_dt;
        DynamicsWorldController         *m_DynamicsWorldController;
        std::vector <DynamicObjectPtr>  m_DynamicObjects;
        std::vector <ParticlePtr>       m_Particles;
        std::vector <ConstraintPtr>     m_Constraints;
        std::vector <Plane>             m_Planes;

        std::vector<QVector3D*>         m_debugLines;

        HashGrid m_hashGrid;
        CollisionDetection m_CollisionDetect;

        Scene *m_scene;

};

#endif // DYNAMICSWORLD_H
