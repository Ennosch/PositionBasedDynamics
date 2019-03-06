#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include  <QDebug>
#include <QVector3D>

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

class OtherConstraint : public AbstractConstraint
{
public:
    OtherConstraint(){};
    float constraintFunction(){qDebug()<<" OtherSpace C"; return 3.0;}
//    float constraintFunction(const QVector3D &_p){return 5.5;}
};

#endif // CONSTRAINT_H






















