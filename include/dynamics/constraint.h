#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include  <QDebug>
#include <QVector3D>
#include <eigen3/Eigen/Dense>

#include "dynamics/abstractconstraint.h"
#include "dynamics/particle.h"
#include "utils.h"


//class AbstractConstraint
//{
//public:
//    AbstractConstraint();
//    inline virtual void project(){}
//    inline virtual float constraintFunction(){qDebug()<<" Abstract C"; return 1.0;}
//    inline virtual QVector3D deltaP(){}
//};

/*
 *          constraint
 *
 *          deltaP - calc delta and return
 *
 *          project - calc delta and apply to particle
 *
 */

//using namespace Eigen;

class HalfSpaceConstraint : public AbstractConstraint
{
public:
    HalfSpaceConstraint(const QVector3D &_p, const QVector3D &_qc, const QVector3D &_n);
    HalfSpaceConstraint(const ParticlePtr _p, const QVector3D &_qc, const QVector3D &_n);

    void project();
    float constraintFunction(){qDebug()<<" HalfSpace C"; return 2.0;}
    float constraintFunction(const QVector3D &_p);
    QVector3D deltaP();

private:
    float d;
    ParticlePtr pptr;
    QVector3D n, p, dp, qc;
};

class PinConstraint : public AbstractConstraint
{
public:
    PinConstraint(const ParticlePtr _particle, const QVector3D &_pos );
    void project();
    float constraintFunction();
    void setPositon(const QVector3D &_pos);
    QVector3D deltaP();
private:
    QVector3D pinPosition;
    ParticlePtr particle;
};

class ParticleParticleConstraint : public AbstractConstraint
{
public:
    ParticleParticleConstraint(const ParticlePtr _p1, const ParticlePtr _p2);
    ParticleParticleConstraint(const ParticlePtr _p1, const ParticlePtr _p2, float _d);
    void project();
    float constraintFunction();
    QVector3D deltaP();
private:
    int id;
    float d;
    ParticlePtr pptr1, pptr2;
};

class DistanceEqualityConstraint : public AbstractConstraint
{
public:
    DistanceEqualityConstraint(const ParticlePtr _p1, const ParticlePtr _p2 );

    float constraintFunction();
    QVector3D deltaP();
    void project();

    void setRestLength(float _d);
    float getRestLength();

private:
    float d;
    QVector3D Vp1, Vp2;
    ParticlePtr pptr1, pptr2;
};

class ShapeMatchingConstraint : public AbstractConstraint, public std::enable_shared_from_this<ShapeMatchingConstraint>
//class ShapeMatchingConstraint : public AbstractConstraint
{
public:
    ShapeMatchingConstraint();
    ShapeMatchingConstraint(RigidBody *_rigidbody);
    ShapeMatchingConstraint(const std::vector<ParticleWeakPtr> &_particles);
    void project();
    float constraintFunction();

private:
    struct LocalParticle{
        ParticleWeakPtr particle;
        QVector3D localX;
    };

    Transform m_transform;
    QMatrix4x4 modelMatrix;

    std::vector<LocalParticle> m_configuration;
    RigidBody *m_rb;
    std::vector<ParticleWeakPtr> m_particlePtrs;
    QVector3D cm, cmOrigin;
//    QMatrix3x3 Ap, Aq;
    Eigen::Matrix3f Ap, Aq;
};

/*WIP
 *
 * investigate memory leaking when adding particles from shapeMatching constraint / rigidbody
 *
 *  push sm constraint (this ) as smartPointer to the particles
 *
 */

#endif // CONSTRAINT_H






















