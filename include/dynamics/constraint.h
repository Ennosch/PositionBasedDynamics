#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include  <QDebug>
#include <QVector3D>
#include <eigen3/Eigen/Dense>

#include "dynamics/abstractconstraint.h"
#include "dynamics/particle.h"
#include "utils.h"

typedef double Real;
//using Matrix3r = Eigen::Matrix<Real, 3, 3>;
//using Vector3r = Eigen::Matrix<Real, 3, 1>;
//const Real eps = static_cast<Real>(1e-6);
using Matrix3r = Eigen::Matrix3f;
using Vector3r = Eigen::Matrix<float, 3, 1>;

const double eps = 1e-6;

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

//class ShapeMatchingConstraint : public AbstractConstraint, public std::enable_shared_from_this<ShapeMatchingConstraint>
class ShapeMatchingConstraint : public AbstractConstraint
{
public:
    ShapeMatchingConstraint();
    ShapeMatchingConstraint(RigidBody *_rigidbody);
    void projectOld();
    void project1D();
    void project();
    float constraintFunction();

private:
    struct smParticle{
        // Vec3 localP  (restPose)
        // Vec3 target  (cached)
        QVector3D localRest, target;
        // Vec3 *p->    (to be updated)
        QVector3D *p;
    };
    std::vector< smParticle > m_ShapeParticles;
    std::vector< ParticlePtr> m_particles;

    QVector3D cm, cmTarget,cmOrigin;

    Transform m_transform;
    QMatrix4x4 modelMatrix;

    RigidBody *m_rb;
    Eigen::Matrix3f Ap, Aq;
};


void polarDecompositionStable(const Matrix3r &M, const double tolerance, Matrix3r &R);

double oneNorm(const Matrix3r &A);

double infNorm(const Matrix3r &A);


/*WIP
 *
 * investigate memory leaking when adding particles from shapeMatching constraint / rigidbody
 *
 *  push sm constraint (this ) as smartPointer to the particles
 *
 */

#endif // CONSTRAINT_H






















