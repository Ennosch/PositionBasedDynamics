#include "dynamics/constraint.h"




//AbstractConstraint::AbstractConstraint()
//{

//}

HalfSpaceConstraint::HalfSpaceConstraint(
        const QVector3D &_p,
        const QVector3D &_qc,
        const QVector3D &_n) :
    n(_n),
    p(_p),
    qc(_qc)
{
//            qDebug()<<"INit Halfspace Constraint";
}

void HalfSpaceConstraint::project()
{
//    QVector3D::dotProduct((p-qc),  n);
    dp = constraintFunction(p) * n;
//    p += dp;
}

float HalfSpaceConstraint::constraintFunction(const QVector3D &_p)
{
    float C = QVector3D::dotProduct((p-qc),  n);;
    return C;
}

QVector3D HalfSpaceConstraint::deltaP()
{
//    return constraintFunction() * n;
    return constraintFunction(p) * -n;
}


PointConstraint::PointConstraint(const QVector3D &_p) :
    p(_p)
{

}

float PointConstraint::constraintFunction()
{
    return 1.0;
}

QVector3D PointConstraint::deltaP()
{
    return p;
}

DistanceEqualityConstraint::DistanceEqualityConstraint(const ParticlePtr _p1, const ParticlePtr _p2) :
    pptr1(_p1),
    pptr2(_p2)
{

}

DistanceEqualityConstraint::DistanceEqualityConstraint(const Particle &_p1, const Particle &_p2):
    p1(_p1),
    p2(_p2)
{

}

float DistanceEqualityConstraint::constraintFunction()
{
//    return ((p1.p - p2.p).length() - d);
    return ((pptr1->p - pptr2->p).length() - d);
}

QVector3D DistanceEqualityConstraint::deltaP()
{

}

void DistanceEqualityConstraint::project()
{

//    p1.p = QVector3D(0,0.1,0);
    float w1, w2;
    QVector3D dp1, dp2, p1, p2;

    p1 = pptr1->p;
    p2 = pptr2->p;
    w1 = pptr1->w;
    w2 = pptr2->w;

    dp1 =  -w1/(w1 + w2) * constraintFunction() * ((p1 - p2) / (p1-p2).length());
    dp2 =  +w2/(w1 + w2) * constraintFunction() * ((p1 - p2) / (p1-p2).length());

    pptr1->p += (dp1 / 2);
    pptr2->p += (dp2 / 2);

    float c2 = constraintFunction();

//    qDebug()<<"projecting";
}

void DistanceEqualityConstraint::setRestLength(float _d)
{
    d = _d;
}

float DistanceEqualityConstraint::getRestLength()
{
    return d;
}




















