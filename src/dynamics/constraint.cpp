#include "dynamics/constraint.h"
#include "dynamics/rigidBody.h"
#include "dynamics/rigidBodyGrid.h"


#include "parameters.h"

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
    m_type = HALFSPACE;
    //            qDebug()<<"INit Halfspace Constraint";
}

HalfSpaceConstraint::HalfSpaceConstraint(const ParticlePtr _p, const QVector3D &_qc, const QVector3D &_n) :
        pptr(_p),
        n(_n),
        qc(_qc)
{
    m_type = HALFSPACE;
    p = pptr->p;
}

void HalfSpaceConstraint::project()
{
//    mlog<<"HalfSpaceConstraint::project()";
    if(constraintFunction(pptr->p) > 0)
        return;
    pptr->p += deltaP();
}

float HalfSpaceConstraint::constraintFunction()
{
//    qDebug()<<" HalfSpace C"; return 2.0;
    return constraintFunction(pptr->p);
}

float HalfSpaceConstraint::constraintFunction(const QVector3D &_p)
{
    if(_p == qc)
        return 0;
    float C = QVector3D::dotProduct((_p-qc),  n);
    return C;
}

QVector3D HalfSpaceConstraint::deltaP()
{
    return constraintFunction(pptr->p) * -n;
}

HalfSpacePreConditionConstraint::HalfSpacePreConditionConstraint(const ParticlePtr _p, const QVector3D &_qc, const QVector3D &_n) :
    pptr(_p),
    n(_n),
    qc(_qc)
{
    m_type = HALFSPACE_PRE;
}

void HalfSpacePreConditionConstraint::project()
{
    float c = constraintFunction();
    pptr->x += c * -n;
    pptr->p += c * -n;
}

float HalfSpacePreConditionConstraint::constraintFunction()
{
    if(pptr->x == qc)
        return 0;
    float C = QVector3D::dotProduct((pptr->x-qc),  n);
    return C;
}

PinConstraint::PinConstraint(const ParticlePtr _particle, const QVector3D &_pos) :
    pinPosition(_pos) ,
    particle(_particle)
{
    m_Particles.push_back(_particle);
    m_type = PIN;
}

void PinConstraint::project()
{
    particle->p = pinPosition;
}

float PinConstraint::constraintFunction()
{
    return 0.0;
}

void PinConstraint::setPositon(const QVector3D &_pos)
{
    pinPosition = _pos;
}

QVector3D PinConstraint::deltaP()
{
    return pinPosition;
}

ParticleParticleConstraint::ParticleParticleConstraint(const ParticlePtr _p1, const ParticlePtr _p2) :
    pptr1(_p1),
    pptr2(_p2)
{
    d = (pptr2->p - pptr1->p).length() - (pptr1->radius() + pptr2->radius());
    m_type = PARTICLEPARTICLE;
}

ParticleParticleConstraint::ParticleParticleConstraint(const ParticlePtr _p1, const ParticlePtr _p2, float _d) :
    pptr1(_p1),
    pptr2(_p2),
    d(_d)
{
    m_type = PARTICLEPARTICLE;
}

void ParticleParticleConstraint::project()
{
    if(!m_dirty)
        return;

    QVector3D n = (pptr2->p - pptr1->p).normalized();
    float totalWeight = pptr2->w + pptr1->w;
    d = constraintFunction();

    QVector3D collisionNormal = d * n;
    float oldLength = collisionNormal.length();

    if(pptr1->collisionVector != QVector3D(0,0,0))
    {
//        collisionNormal = pptr1->collisionVector * 1.0;
//        collisionNormal = pptr2->collisionVector.normalized() * oldLength;
//        mlog<<"p1 : "<<collisionNormal.length()<<" - "<<oldLength;
    }
    else if(pptr2->collisionVector != QVector3D(0,0,0))
    {
//        collisionNormal = pptr2->collisionVector * 1.0;

        float gardLength = pptr2->collisionVector.length();
//        collisionNormal = pptr2->collisionVector.normalized() * oldLength;
        collisionNormal = -pptr2->collisionVector ;
//        collisionNormal = -pptr2->collisionVector ;

//        mlog<<"p2 : "<<pptr2->ID<<" collNormal l: "<<collisionNormal.length()<<" d: "<<oldLength<<" gradLength:"<< gardLength;
    }

    pptr1->p += (pptr1->w / totalWeight) * collisionNormal;
    pptr2->p += (pptr2->w / totalWeight) * -collisionNormal;

    m_dirty = false;
}


float ParticleParticleConstraint::constraintFunction()
{
    return  (pptr2->p - pptr1->p).length() - (pptr1->radius() + pptr2->radius());;
}

QVector3D ParticleParticleConstraint::deltaP()
{
    return QVector3D(0,0,0);
}

ParticleParticlePreConditionConstraint::ParticleParticlePreConditionConstraint(const ParticlePtr _p1, const ParticlePtr _p2) :
    pptr1(_p1),
    pptr2(_p2)
{
    d = (pptr2->p - pptr1->p).length() - (pptr1->radius() + pptr2->radius());
    m_type = PARTICLEPARTICLE_PRE;
}

void ParticleParticlePreConditionConstraint::project()
{
    if(!m_dirty)
        return;

    return;

    QVector3D n = (pptr2->x - pptr1->x).normalized();
    float totalWeight = pptr2->w + pptr1->w;

    QVector3D collisionNormal = d * n;

//    if(pptr1->collisionVector != QVector3D(0,0,0))
//    {
//        collisionNormal = pptr1->collisionVector;
//    }
//    else if(pptr2->collisionVector != QVector3D(0,0,0))
//    {
//        collisionNormal = pptr2->collisionVector;
//    }

    QVector3D correctionA = (pptr1->w / totalWeight) *  collisionNormal;
    QVector3D correctionB = (pptr2->w / totalWeight) * -collisionNormal;

    pptr1->x += correctionA;
    pptr2->x += correctionB;

    pptr1->p += correctionA;
    pptr2->p += correctionB;

    m_dirty = false;
}

float ParticleParticlePreConditionConstraint::constraintFunction()
{

}

QVector3D ParticleParticlePreConditionConstraint::deltaP()
{
    return QVector3D(0,0,0);
}




DistanceEqualityConstraint::DistanceEqualityConstraint(const ParticlePtr _p1, const ParticlePtr _p2)
    :
    pptr1(_p1),
    pptr2(_p2)
{
    m_Particles.push_back(_p1);
    m_Particles.push_back(_p2);
    m_type = DISTANCE;
}


float DistanceEqualityConstraint::constraintFunction()
{
//    return ((p1.p - p2.p).length() - d);
    springDir = (pptr1->p - pptr2->p);
    springLength = springDir.length();

    return (springLength - d);
}

QVector3D DistanceEqualityConstraint::deltaP()
{

}

void DistanceEqualityConstraint::project()
{
    if(!m_dirty)
        return;

//    p1.p = QVector3D(0,0.1,0);
    float w1, w2;
    QVector3D dp1, dp2, p1, p2;

    float c1 = constraintFunction();


    float compressR = distanceConstraintCompressR;
    float strechR = distanceConstraintStrechR;
    float resistance;

    if(springLength > d)
        resistance = strechR;
    else
        resistance = compressR;

    p1 = pptr1->p;
    p2 = pptr2->p;
    w1 = pptr1->w;
    w2 = pptr2->w;

    QVector3D changeDir = springDir / springLength;

    dp1 =  -(w1/(w1 + w2)) * c1 * changeDir * resistance;
    dp2 =  +(w2/(w1 + w2)) * c1 * changeDir * resistance;

    pptr1->p += (dp1 * 1.0);
    pptr2->p += (dp2 * 1.0);

    m_dirty = false;

}

void DistanceEqualityConstraint::setRestLength(float _d)
{
    d = _d;
}

float DistanceEqualityConstraint::getRestLength()
{
    return d;
}

ShapeMatchingConstraint::ShapeMatchingConstraint()
{
    m_type = SHAPEMATCH;
}

ShapeMatchingConstraint::ShapeMatchingConstraint(RigidBody *_rigidbody)
{
    m_type = SHAPEMATCH;
    m_rb = _rigidbody;
    m_rbg = nullptr;
    preCompute(_rigidbody->m_particles.size(), _rigidbody->m_particles, _rigidbody->m_restShape);
}

ShapeMatchingConstraint::ShapeMatchingConstraint(RigidBodyGrid *_rigidbody)
{
    m_type = SHAPEMATCH_RIGID;
    m_rb = nullptr;
    m_rbg = _rigidbody;
    preCompute(_rigidbody->m_particles.size(), _rigidbody->m_particles, _rigidbody->m_restShape);
}

void ShapeMatchingConstraint::project()
{
    if(!m_dirty)
        return;

    cm.setZero();
    for(auto p : m_particles)
    {
        cm += Eigen::Vector3f(p->p.x(), p->p.y(), p->p.z());
    }
    cm /= m_particles.size();

    Apq.setZero();
    for(int i=0; i < m_particles.size(); i++)
    {
        Eigen::Vector3f pi = Eigen::Vector3f(m_particles[i]->p.x(), m_particles[i]->p.y(), m_particles[i]->p.z()) - cm;
        Eigen::Vector3f qi = m_restPositions[i] - cmOrigin;
        Apq += pi * qi.transpose();
    }

    Eigen::Matrix3f A = Apq * Aqq.inverse();
    Eigen::JacobiSVD<Eigen::MatrixXf> svd(A, Eigen::ComputeThinU | Eigen::ComputeThinV);

    R = svd.matrixU() * svd.matrixV().transpose();

    if(m_type == SHAPEMATCH_RIGID)
    {
        m_rbg->m_t.setToIdentity();
//        QMatrix4x4 qtR =
//        m_rbg->m_t.rotate(R);
    }


    qPrev = q;
    q = R;

//    mlog<<m_particles.size();
//    std::cout<<R<<std::endl;

//    q = q.normalized();
//    mlog<<"q: "<<q.x()<< q.y()<< q.z()<< q.w()<< "  qPrev"<<qPrev.x()<<qPrev.y()<<qPrev.z()<<qPrev.w();

//    Eigen::Quaternion<float, Eigen::AutoAlign> quat(Eigen::MatrixX3f R);

    for(int i=0; i < m_particles.size(); i++)
    {
        Eigen::Vector3f qi =  m_restPositions[i] - cmOrigin;
//        Eigen::Vector3f gi = (R * qi) + (cm - cmOrigin);
        Eigen::Vector3f gi = (R * qi) + (cm);
        m_particles[i]->p = QVector3D(gi.x(), gi.y(), gi.z());
    }
    m_dirty = false;
}

float ShapeMatchingConstraint::constraintFunction()
{
    return 0.0;
}


void ShapeMatchingConstraint::preCompute(int numParticles, std::vector<ParticleWeakPtr> &_particles, std::vector<QVector3D> &_restShape)
{
    q = Eigen::Quaternionf(Eigen::AngleAxisf(0, Eigen::Vector3f(0, 0, 0)));
    qPrev = q;

    cmOrigin = Eigen::Vector3f(0,0,0);
    cm = Eigen::Vector3f(0,0,0);
    for(int i=0; i < _particles.size(); i++)
    {
        auto p = _particles[i];
        if(auto particle = p.lock())
        {
            m_particles.push_back(particle);
            Eigen::Vector3f x0 = Eigen::Vector3f(_restShape[i].x(), _restShape[i].y(), _restShape[i].z());
            m_restPositions.push_back(x0);
            cmOrigin += x0;
        }
    }
    cmOrigin /= m_particles.size();

//    Aqq.setZero();
    Aqq.setIdentity();
    for(int i=0; i < _particles.size(); i++)
    {
        Eigen::Vector3f x0 = Eigen::Vector3f(_restShape[i].x(), _restShape[i].y(), _restShape[i].z());
        Eigen::Vector3f qi = x0 - cmOrigin;
        Eigen::Matrix3f _dot = qi * qi.transpose();
        Aqq += _dot;
    }
}


void polarDecompositionStable(const Matrix3r &M, const double tolerance, Matrix3r &R)
{
    Matrix3r Mt = M.transpose();
    double Mone = oneNorm(M);
    double Minf = infNorm(M);
    double Eone;
    Matrix3r MadjTt, Et;
    do
    {
        MadjTt.row(0) = Mt.row(1).cross(Mt.row(2));
        MadjTt.row(1) = Mt.row(2).cross(Mt.row(0));
        MadjTt.row(2) = Mt.row(0).cross(Mt.row(1));

        double det = Mt(0,0) * MadjTt(0,0) + Mt(0,1) * MadjTt(0,1) + Mt(0,2) * MadjTt(0,2);

        if (fabs(det) < 1.0e-12)
        {
            Vector3r len;
            unsigned int index = 0xffffffff;
            for (unsigned int i = 0; i < 3; i++)
            {
                len[i] = MadjTt.row(i).squaredNorm();
                if (len[i] > 1.0e-12)
                {
                    // index of valid cross product
                    // => is also the index of the vector in Mt that must be exchanged
                    index = i;
                    break;
                }
            }
            if (index == 0xffffffff)
            {
                R.setIdentity();
                return;
            }
            else
            {
                Mt.row(index) = Mt.row((index + 1) % 3).cross(Mt.row((index + 2) % 3));
                MadjTt.row((index + 1) % 3) = Mt.row((index + 2) % 3).cross(Mt.row(index));
                MadjTt.row((index + 2) % 3) = Mt.row(index).cross(Mt.row((index + 1) % 3));
                Matrix3r M2 = Mt.transpose();
                Mone = oneNorm(M2);
                Minf = infNorm(M2);
                det = Mt(0,0) * MadjTt(0,0) + Mt(0,1) * MadjTt(0,1) + Mt(0,2) * MadjTt(0,2);
            }
        }

        const double MadjTone = oneNorm(MadjTt);
        const double MadjTinf = infNorm(MadjTt);

        const double gamma = sqrt(sqrt((MadjTone*MadjTinf) / (Mone*Minf)) / fabs(det));

//        const double g1 = gamma* static_cast<Real>(0.5);
        const double g1 = 0.5;
//        const double g2 = static_cast<Real>(0.5) / (gamma*det);
        const double g2 = (0.5) / (gamma*det);

        for (unsigned char i = 0; i < 3; i++)
        {
            for (unsigned char j = 0; j < 3; j++)
            {
                Et(i,j) = Mt(i,j);
                Mt(i,j) = g1*Mt(i,j) + g2*MadjTt(i,j);
                Et(i,j) -= Mt(i,j);
            }
        }

        Eone = oneNorm(Et);

        Mone = oneNorm(Mt);
        Minf = infNorm(Mt);
    } while (Eone > Mone * tolerance);

    // Q = Mt^T
    R = Mt.transpose();
}




double oneNorm(const Matrix3r &A)
{
    const double sum1 = fabs(A(0,0)) + fabs(A(1,0)) + fabs(A(2,0));
    const double sum2 = fabs(A(0,1)) + fabs(A(1,1)) + fabs(A(2,1));
    const double sum3 = fabs(A(0,2)) + fabs(A(1,2)) + fabs(A(2,2));
    double maxSum = sum1;
    if (sum2 > maxSum)
        maxSum = sum2;
    if (sum3 > maxSum)
        maxSum = sum3;
    return maxSum;
}


double infNorm(const Matrix3r &A)
{
    const double sum1 = fabs(A(0, 0)) + fabs(A(0, 1)) + fabs(A(0, 2));
    const double sum2 = fabs(A(1, 0)) + fabs(A(1, 1)) + fabs(A(1, 2));
    const double sum3 = fabs(A(2, 0)) + fabs(A(2, 1)) + fabs(A(2, 2));
    double maxSum = sum1;
    if (sum2 > maxSum)
        maxSum = sum2;
    if (sum3 > maxSum)
        maxSum = sum3;
    return maxSum;
}



FrictionConstraint::FrictionConstraint(const ParticlePtr _p1, const ParticlePtr _p2)
    : pptr1(_p1),
      pptr2(_p2)
{
    m_collisionNormal = (_p2->x - _p1->x).normalized();

}

void FrictionConstraint::project()
{
    if(!m_dirty)
        return;

    QVector3D td, xj;
    td = (pptr1->p - pptr1->x)  -  (pptr2->p - pptr2->x) - constraintFunction() * m_collisionNormal;
    float tdLength = td.length();
    float totalWeight = pptr1->w + pptr2->w;

    float usd = frictionConstraintStaticF;
    float ukd = frictionConstraintDynamicF;

//    return;
    if(tdLength < usd)
    {
        xj = td;
        pptr1->p += (pptr1->w / totalWeight) * -xj;
    }
    else
    {
        xj = td * std::min( (ukd / tdLength) , float(1.0));
        pptr1->p += (pptr1->w / totalWeight) * -xj;
    }

    pptr2->p += (pptr2->w / totalWeight) * xj;
//    mlog<<"hello friction";
    m_dirty = false;
}

float FrictionConstraint::constraintFunction()
{
    return QVector3D::dotProduct((pptr1->p - pptr1->x)  -  (pptr2->p - pptr2->x), m_collisionNormal);
}

HalfSpaceFrictionConstraint::HalfSpaceFrictionConstraint(const ParticlePtr _p1, const QVector3D _o, const QVector3D _n) :
    pptr1(_p1),
    m_collisionNormal(_n),
    planeOrigin(_o)
{
//    mlog<<"HalfSfriction ctro";
}

void HalfSpaceFrictionConstraint::project()
{
    if(!m_dirty)
        return;
    QVector3D td, xj;
    td = (pptr1->p - pptr1->x)   - constraintFunction() * m_collisionNormal;
    float tdLength = td.length();

    float usd = 0.5;
    float ukd = 0.5;

    if(tdLength < usd)
    {
//        mlog<<"project usd";
        pptr1->p += -td;
    }
    else
    {
//         mlog<<"project ukd";
        pptr1->p += -td * std::min( (ukd / tdLength) , float(1.0));
    }
    m_dirty = false;
}

float HalfSpaceFrictionConstraint::constraintFunction()
{
    return QVector3D::dotProduct((pptr1->p - pptr1->x) , m_collisionNormal);
}

PinTogetherConstraint::PinTogetherConstraint(std::vector<ParticlePtr> &_particleVec)
{
    m_particles = _particleVec;
    mlog<<"PinConstraint size"<<m_particles.size();
    m_type = PINTOGETHER;

    for(auto p : m_particles)
    {
        m_Particles.push_back(p);
    }
}

void PinTogetherConstraint::project()
{
    m_avrgPos = QVector3D(0,0,0);

    for(auto p : m_particles)
    {
        m_avrgPos += p->p;
    }
    m_avrgPos = m_avrgPos / m_particles.size();

    for(auto p : m_particles)
    {
        p->p = m_avrgPos;
    }
}

float PinTogetherConstraint::constraintFunction()
{
    return 1.0;
}

