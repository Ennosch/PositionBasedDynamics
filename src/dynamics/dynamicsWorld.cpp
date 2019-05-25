
#include "dynamics/dynamicsWorld.h"

#include "Scene.h"

#include <Qdebug>

Eigen::Matrix3f QMatrix3toEigen(const QMatrix3x3 &_qMat);
QMatrix3x3 EigenMatrix3toQt(const Eigen::Matrix3f &_eMat);


DynamicsWorld::DynamicsWorld()
    : m_hashGrid()
{
    qDebug()<<"DynaicsWorld ctor";
    m_simulate = false;
    m_dt = 0.1;
    m_frameCount = 0;
    m_DynamicsWorldController = new DynamicsWorldController(this);
}

void DynamicsWorld::initialize()
{
    qDebug()<<"DynamicsWorld initialize ";
//    Eigen::Vector3d test(1,2,3);

//    mlog<<"helloo"<<test.x()<<test.y();

    m_CollisionDetect = CollisionDetection();
    Plane groundPlane;
    groundPlane.Normal = QVector3D(0, 1, 0);
    groundPlane.Offset = QVector3D(0, 0, 0);
    addPlane(groundPlane);

//    auto nSpring = std::make_shared<DistanceEqualityConstraint>(m_Particles[0], m_Particles[1]);

    for( ParticlePtr p : m_Particles)
    {
        p->pp = QVector3D(0,0,0);
    }
}

void DynamicsWorld::initialize(Scene *_scene)
{
    m_scene = _scene;
    initialize();
}

void DynamicsWorld::update()
{

    float dt = m_dt;

    if(!m_simulate)
        return;


    // PBD Loop start
    // explicit Euler integration step (5)


    for( ParticlePtr p : m_Particles)
    {
        // e.G. gravity 0, 1, 0
        QVector3D forceExt = QVector3D(0, -10, 0);
        p->v = p->v + dt * p->w*forceExt;
    }

    // damp Velocities (6)
    pbdDamping();

    for( ParticlePtr p : m_Particles)
    {
        p->p = p->x + dt * p->v;
    }

    //  Collision Detection (8)
    //  spatial Hash for particle particle collision
    m_hashGrid.clear();
    for( ParticlePtr p : m_Particles)
    {
        //    Start with empty multimap
        //    For each particle P
        //      Determine which cell will contain P
        //      Retrieve all particles in P‘s cell and the eight neighboring cells
        //      Calculate distance from P to each of these and compare to d
        //      Insert P into multimap
//            qDebug()<<"p: "<<p->ID<<"x: "<<p->position().x()<<"y: "<<p->position().y()<<"z: "<<p->position().z();
        int3 pCell = m_hashGrid.pointToCell(
                    p->position().x(),
                    p->position().y(),
                    p->position().z() );

//            qDebug()<<"Position: "<<p->position().x()<<p->position().y()<<p->position().z();
//            qDebug()<<"Cell: "<<pCell.i<<pCell.j<<pCell.k;

        size_t pHash = m_hashGrid.hashFunction(pCell);
           p->setHash(pHash);
        m_hashGrid.insert(pHash, p);
        // get all neightbouring particles
        std::list<ParticlePtr> neighbourParticles = m_hashGrid.cellNeighbours(pCell);
        for(ParticlePtr n : neighbourParticles)
//        for( ParticlePtr n : m_Particles)
        {
//            bool isNonCollide = std::includes
            bool isNonCollider = false;
            for(auto ntest : p->m_NonCollisionParticles)
            {
                if(ntest.lock() == n)
                {
                    isNonCollider =true;
                }
            }
            if(isNonCollider)
                continue;

            if(n != p)
                // check predicted postion for particle particle collisions
                checkSphereSphere(p,n);
        }
        checkSpherePlane(p, m_Planes[0]);
    }

    // Constraint dirty to do something
        for( ParticlePtr p : m_Particles)
        {

            for( ConstraintWeakPtr c : p->m_Constraints)
            {
                if(auto constraint = c.lock()){
                    constraint->setDirty(true);
                }
            }
        }

//    for(int i = 0 ; i <10; i++)
//    {
//        for( ParticlePtr p : m_Particles)
//        {
//            for( ConstraintWeakPtr c : p->m_Constraints)
//            {
//    //            p->p += c->deltaP();
//    //            auto test = c.lock();
//    //            bool test2 = c.expired();
//                if(auto constraint = c.lock()){
//    //                for(int i=0; i<=5; i++)
//                    {
//                        constraint->project();
//                    }
//                }
//            }
//        }
//    }

    // Solver Iteration (9)
    for( ParticlePtr p : m_Particles)
    {
        for( ConstraintWeakPtr c : p->m_Constraints)
        {
//            p->p += c->deltaP();
//            auto test = c.lock();
//            bool test2 = c.expired();
            if(auto constraint = c.lock()){
//                for(int i=0; i<=5; i++)
                {
                    constraint->project();
                }
            }
        }

        for( ConstraintPtr c : p->m_CollisionConstraints)
        {
//            if(auto constraint = c.lock())
//                p->p += constraint->deltaP();
//            p->p += c->deltaP();
            c->project();
        }
        p->m_CollisionConstraints.clear();

        for( ConstraintPtr c : p->m_CollisionConstraints_B)
        {
//            if(auto constraint = c.lock())
//                p->p += constraint->deltaP();
//            p->p += c->deltaP();
            c->project();
        }
        p->m_CollisionConstraints_B.clear();
    }



    // Apply correction (13,14)
    for( ParticlePtr p : m_Particles)
    {
        p->v = (p->p - p->x) / dt;
        p->x = p->p;
//        p->x = p->x + QVector3D(0, -0.01, 0);
    }

//     modify velocity (16)
//    for( ParticlePtr p : m_Particles)
//    {
//        p->v = p->v + p->pp;
//        p->pp = QVector3D(0,0,0);
//    }
    m_frameCount++;
}

void DynamicsWorld::info()
{
    qDebug()<<"p1: "<<m_Particles[0]->x<<m_Particles[0]->ID<<m_Particles[0]->r;
}

DynamicsWorldController* DynamicsWorld::controller()
{
    if(m_DynamicsWorldController)
        return m_DynamicsWorldController;
    return nullptr;
}

void DynamicsWorld::setSimulate(bool _isSimulating)
{
    m_simulate = _isSimulating;
}

void DynamicsWorld::step()
{
    m_simulate = true;
    update();
    m_simulate = false;
}

void DynamicsWorld::pbdDamping()
{
    int i = 0;

    for(auto dObject : m_DynamicObjects)
    {
        i++;
        QVector3D xcm, vcm, ri, L, w;
        QMatrix3x3 I;
        int totalMass = 0;

        if(dObject->getParticles().size() < 1)
            continue;

        for(auto p : dObject->getParticles())
        {
            if(auto particle = p.lock())
            {
                xcm += particle->m * particle->x;
                vcm += particle->m * particle->v;
                totalMass += particle->m;
            }
        }
        xcm /= totalMass;
        vcm /= totalMass;

//        mlog<<xcm<<vcm;
//        continue;

        for(auto p : dObject->getParticles())
        {
            if(auto particle = p.lock())
            {
                ri = particle->x - xcm;
                L += QVector3D::crossProduct(ri, (particle->m * particle->v));
                QMatrix3x3 rt;
                rt(0,0) =            0 ; rt(0,1) =       -ri[2] ; rt(0,2) =        ri[1] ;
                rt(1,0) =         ri[2]; rt(1,1) =            0 ; rt(1,2) =       -ri[0] ;
                rt(2,0) =        -ri[1]; rt(2,1) =        ri[0] ; rt(2,2) =            0 ;
                rt *= particle->m;

                Eigen::Matrix3f rtMat = QMatrix3toEigen(rt);
                Eigen::Matrix3f IMat = rtMat * rtMat.transpose() * particle->m;
                I += EigenMatrix3toQt(IMat);
            }
        }
        Eigen::Vector3f wM = QMatrix3toEigen(I).inverse() * Eigen::Vector3f(L.x(), L.y(), L.z());
        w = QVector3D(wM.x(), wM.y(), wM.z());

        int i = 0;
        for(auto p : dObject->getParticles())
        {
            if(auto particle = p.lock())
            {
                ri = particle->x - xcm;
                QVector3D dv = vcm + (QVector3D::crossProduct(w, ri)) - particle->v;
                i++;
                if(i == 1){
//                    mlog<<"  v: "<< particle->v<< "ri "<<ri<< "vcm: "<<vcm<<"xcm:"<<xcm<<"dv: "<<dv<<" w x ri: "<<QVector3D::crossProduct(w, ri);
                }
                particle->v +=  (0.0 * dv);
            }
        }
    }
}

/*
 *
 * from https://stackoverflow.com/questions/2047414/advantages-of-stdfor-each-over-for-loop
 *
 * for_each(a.begin(), a.end(), a_item_handler);
is better than

for(auto& item: a) {
    a_item_handler(a);
}
*/

void DynamicsWorld::addDynamicObject(pSceneOb _sceneObject)
{

}

DynamicObjectPtr DynamicsWorld::addDynamicObjectAsParticle(pSceneOb _sceneObject)
{
    pCount++;
    QVector3D pos = _sceneObject->getPos();
    auto pDynamicObject = std::make_shared<Particle>(pos.x(), pos.y(), pos.z(), pCount);
    pDynamicObject->setRadius(_sceneObject->getRadius());
    m_Particles.push_back(pDynamicObject);
    m_DynamicObjects.push_back(pDynamicObject);
    _sceneObject->makeDynamic(pDynamicObject);
    return pDynamicObject;
}

DynamicObjectPtr DynamicsWorld::addDynamicObjectAsRigidBody(pSceneOb _sceneObject)
{

    if(!_sceneObject->model())
        return nullptr;

    auto nRB = std::make_shared<RigidBody>(_sceneObject->model());
    ModelPtr model = nRB->getModel();
    _sceneObject->setModel(nRB->getModel());

    for(unsigned int i = 0; i < model->getNumShapes(); i++)
    {
        ShapePtr shape = model->getShape(i);
        for(auto point : shape->getPoints())
        {
            QVector3D pos = _sceneObject->getMatrix() * point;
            auto nParticle = std::make_shared<Particle>(pos.x(), pos.y(), pos.z(), 33);
            nParticle->setRadius(_sceneObject->getRadius());
            m_Particles.push_back(nParticle);
            nRB->addParticle(point, nParticle);
            m_scene->addSceneObjectFromParticle(nParticle);
        }
    }
    auto smCstr = nRB->createConstraint();
    m_Constraints.push_back(smCstr);

    m_DynamicObjects.push_back(nRB);

    nRB->updateModelBuffers();
    _sceneObject->makeDynamic(nRB);

    return nRB;
}

DynamicObjectPtr DynamicsWorld::addDynamicObjectAsSoftBody(pSceneOb _sceneObject)
{
    if(!_sceneObject->model())
        return nullptr;

     auto nSB = std::make_shared<SoftBody>(_sceneObject->model());
     ModelPtr model = nSB->getModel();
      _sceneObject->setModel(nSB->getModel());

     for(unsigned int i = 0; i < model->getNumShapes(); i++)
     {
         ShapePtr shape = model->getShape(i);
         for(auto point : shape->getPoints())
         {
             QVector3D pos = _sceneObject->getMatrix() * point;
             auto nParticle = std::make_shared<Particle>(pos.x(), pos.y(), pos.z(), 33);
             m_Particles.push_back(nParticle);
             nSB->addParticle(point, nParticle);
             m_scene->addSceneObjectFromParticle(nParticle);
         }
     }
     std::vector< std::set<int> > constraintIdxs  = nSB->createConstraintNetwork();

     for(auto set : constraintIdxs)
     {
         if(set.size() < 2)
             continue;

         std::set<int>::iterator itA, itB;
         itA = set.begin();
         itB = set.begin();
         std::advance(itB, 1);

         int A = *itA;
         int B = *itB;

         ParticlePtr p1 = nSB->getParticles()[A].lock();
         ParticlePtr p2 = nSB->getParticles()[B].lock();

         float restLength = ((p1->x)-(p2->x)).length();
         std::shared_ptr<DistanceEqualityConstraint> nCstrPtr = addDistanceEqualityConstraint(p1, p2);
         nCstrPtr->setRestLength(restLength);

     };
     // pin rows !
//     for(auto p : nSB->getParticles())
//     {
//         if(ParticlePtr particle = p.lock())
//         {
//             if(particle->x.y() > 12.0)
//             {
//                 QVector3D pos = particle->x;
//                 auto pinCstr = std::make_shared<PinConstraint>(particle, pos);
//                 pinCstr->setPositon(pos);
//                 m_Constraints.push_back(pinCstr);
//                 particle->m_Constraints.push_back(pinCstr);
//             }
//         }
//     }

     m_DynamicObjects.push_back(nSB);
     nSB->turnOffSelfCollision();
     nSB->updateModelBuffers();
     _sceneObject->makeDynamic(nSB);
     return nSB;
//    auto nRB = std::make_shared<SoftBody>(_sceneObject->model());
}

ParticlePtr DynamicsWorld::addParticle(float _x, float _y, float _z)
{
    qDebug()<<"ADDED PARTICLE_____________________";
    pCount++;
    auto pParticle  = std::make_shared<Particle>(_x, _y, _z, pCount);
    m_Particles.push_back(pParticle);
    return pParticle;
}

void DynamicsWorld::addPlane(const Plane &_plane)
{
//    auto pDynamicObject = std::make_shared<DynamicObject>();
    m_Planes.push_back(_plane);
}

void DynamicsWorld::checkSphereSphere(const ParticlePtr p1, const ParticlePtr p2)
{
    // Check predicted positon
    float d;
    if(m_CollisionDetect.checkSphereSphere(p1->p, p2->p, d,p1->radius(), p2->radius())){
        addParticleParticleConstraint(p1, p2);
        addFrictionConstraint(p1, p2);
    }
}

void DynamicsWorld::generateData()
{

//    auto rb = m_DynamicObjects[0];

//    //    for(int i=0; i< rb->getParticles().size(); i++)
//    //    {
//    //        if(ParticlePtr p = rb->getParticles()[i].lock())
//    //        {

//    //        }
//    //    }
//    float av = 100;

//    rb->getParticles()[0].lock()->v = QVector3D(av,-av,0);
//    rb->getParticles()[3].lock()->v = QVector3D(av,-av,0);

//    rb->getParticles()[2].lock()->v = QVector3D(av,av,0);
//    rb->getParticles()[1].lock()->v = QVector3D(av,av,0);

//    rb->getParticles()[4].lock()->v = QVector3D(-av,-av,0);
//    rb->getParticles()[5].lock()->v = QVector3D(-av,-av,0);

//    rb->getParticles()[6].lock()->v = QVector3D(-av,av,0);
//    rb->getParticles()[7].lock()->v = QVector3D(-av,av,0);

}

int DynamicsWorld::frameCount()
{
    return m_frameCount;
}

QVector3D* DynamicsWorld::debugDrawLineData()
{
    return nullptr;
}

void DynamicsWorld::checkSpherePlane(const ParticlePtr p1, const Plane &_plane)
{
    float dist = m_CollisionDetect.distanceFromPointToPlane(p1->p, _plane.Normal, (_plane.Offset + (p1->r *_plane.Normal)));
    if(dist > 0)
        return;
    QVector3D qc = m_CollisionDetect.intersectRayPlane(p1->x, p1->p, _plane.Normal, (_plane.Offset + (p1->r *_plane.Normal)));
//    qc -= (p1->r *_plane.Normal);
    auto hsCstr = std::make_shared<HalfSpaceConstraint>(p1, qc, _plane.Normal);
    p1->m_CollisionConstraints.push_back(hsCstr);
    addHalfSpaceFrictionConstraint(p1, QVector3D(0,0,0), QVector3D(0,1,0));
}

std::shared_ptr<DistanceEqualityConstraint> DynamicsWorld::addDistanceEqualityConstraint(const ParticlePtr _p1, const ParticlePtr _p2)
{
    auto nSpring = std::make_shared<DistanceEqualityConstraint>(_p1, _p2);
    nSpring->setRestLength(2.0);
    m_Constraints.push_back(nSpring);
    _p1->m_Constraints.push_back(nSpring);
    _p2->m_Constraints.push_back(nSpring);

    m_debugLines.push_back(&_p1->x);
    m_debugLines.push_back(&_p2->x);

    return nSpring;
}

void DynamicsWorld::addParticleParticleConstraint(const ParticlePtr _p1, const ParticlePtr _p2)
{
    auto ppCstr = std::make_shared<ParticleParticleConstraint>(_p1, _p2);
    _p1->m_CollisionConstraints.push_back(ppCstr);
    _p2->m_CollisionConstraints.push_back(ppCstr);

}

void DynamicsWorld::addFrictionConstraint(const ParticlePtr _p1, const ParticlePtr _p2)
{
    auto fCstr = std::make_shared<FrictionConstraint>(_p1, _p2);
    _p1->m_CollisionConstraints_B.push_back(fCstr);
    _p2->m_CollisionConstraints_B.push_back(fCstr);
}

void DynamicsWorld::addHalfSpaceFrictionConstraint(const ParticlePtr _p1, const QVector3D _o, const QVector3D _n)
{
    auto fCstr = std::make_shared<HalfSpaceFrictionConstraint>(_p1, QVector3D(0,0,0), QVector3D(0,1,0));
    _p1->m_CollisionConstraints_B.push_back(fCstr);
}

void DynamicsWorld::deleteConstraint(const ConstraintPtr _constraint)
{
    for(auto p : _constraint->m_Particles)
    {
        if(auto particle = p.lock())
        {
            std::vector< ConstraintWeakPtr>::const_iterator it = particle->m_Constraints.begin();
            std::vector< ConstraintWeakPtr>::const_iterator itEnd = particle->m_Constraints.end();
            for(it; it != itEnd; ++it)
            {
                if(it->lock() == _constraint)
                {
                    particle->m_Constraints.erase(it);
                }
            }
        }
    }

    m_Constraints.erase(
                std::remove_if(
                    m_Constraints.begin(),
                    m_Constraints.end(),
                    [&](const ConstraintPtr c){return c == _constraint;}),
                m_Constraints.end()
                );

}


Eigen::Matrix3f QMatrix3toEigen(const QMatrix3x3 &_qMat)
{
    Eigen::Matrix3f mat;
    mat(0,0) = _qMat(0,0); mat(0,1) = _qMat(0,1); mat(0,2) = _qMat(0,2);
    mat(1,0) = _qMat(1,0); mat(1,1) = _qMat(1,1); mat(1,2) = _qMat(1,2);
    mat(2,0) = _qMat(2,0); mat(2,1) = _qMat(2,1); mat(2,2) = _qMat(2,2);
    return mat;
}

QMatrix3x3 EigenMatrix3toQt(const Eigen::Matrix3f &_eMat)
{
    QMatrix3x3 mat;
    mat(0,0) = _eMat(0,0); mat(0,1) = _eMat(0,1); mat(0,2) = _eMat(0,2);
    mat(1,0) = _eMat(1,0); mat(1,1) = _eMat(1,1); mat(1,2) = _eMat(1,2);
    mat(2,0) = _eMat(2,0); mat(2,1) = _eMat(2,1); mat(2,2) = _eMat(2,2);
    return mat;
}




/*
 *
 *
 *
 *
 *
 */











