#include "dynamics/constraint.h"
#include "dynamics/rigidBody.h"




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

HalfSpaceConstraint::HalfSpaceConstraint(const ParticlePtr _p, const QVector3D &_qc, const QVector3D &_n) :
        pptr(_p),
        n(_n),
        qc(_qc)
{
    p = pptr->p;
}

void HalfSpaceConstraint::project()
{
//    QVector3D::dotProduct((p-qc),  n);
//    dp = constraintFunction(p) * n;
//    p += dp;

//    pptr->p += constraintFunction(p) * -n;
    pptr->p += deltaP();
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


PinConstraint::PinConstraint(const ParticlePtr _particle, const QVector3D &_pos) :
    pinPosition(_pos) ,
    particle(_particle)
{
    m_Particles.push_back(_particle);
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
}

ParticleParticleConstraint::ParticleParticleConstraint(const ParticlePtr _p1, const ParticlePtr _p2, float _d) :
    pptr1(_p1),
    pptr2(_p2),
    d(_d)
{

}

void ParticleParticleConstraint::project()
{
    if(!m_dirty)
        return;

    QVector3D n = (pptr2->p - pptr1->p).normalized();

    pptr1->p += (d/2) * n;
    pptr2->p += (d/2) * -n;

     m_dirty = false;
}

float ParticleParticleConstraint::constraintFunction()
{
    return 0.0;
}

QVector3D ParticleParticleConstraint::deltaP()
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

//    qDebug()<<"projecting DistanceEqualityConstraint";
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

}

ShapeMatchingConstraint::ShapeMatchingConstraint(RigidBody *_rigidbody)
{
    m_rb = _rigidbody;
    mlog<<"init SM cstr ptr";


    for(auto p : m_rb->m_particles)
    {
        if(auto particle = p.lock())
        {
            QMatrix4x4 tmpMat = m_rb->getTransfrom();
            QVector3D pLocal = tmpMat.inverted() * particle->x;
            cmOrigin += pLocal;
//            mlog<<"Shape Matching: "<<pLocal;
            LocalParticle dataPair;
            dataPair.localX = pLocal;
            dataPair.particle = p;
            m_configuration.push_back(dataPair);
        }
    }
    cmOrigin /= m_rb->m_particles.size();
//    mlog<<"Shape Matching Div: "<<m_rb->m_particles.size()<<cmOrigin;


}

ShapeMatchingConstraint::ShapeMatchingConstraint(const std::vector<ParticleWeakPtr> &_particles)
{
    m_particlePtrs = _particles;
    mlog<<"init SM cstr";
}

void ShapeMatchingConstraint::project()
{
    if(!m_dirty)
        return;

    for(int j=0; j<1;j ++)
    {

    //     compute center of mass (T)
        int count = 0;
        cm = QVector3D(0,0,0);
        for(auto pair : m_configuration)
        {
            if(auto particle = pair.particle.lock())
            {
                cm += particle->p;
    //            mlog<<particle->x<<count;
                count++;
            }
        }
        cm /= m_configuration.size();
    //    mlog<<(cm );

    //    // compute A (Ax = b)
        Ap.setIdentity();
        Aq.setIdentity();

        for(auto pair : m_configuration)
        {
            if(auto particle = pair.particle.lock())
            {
                QMatrix3x3 tmpP;
                QVector3D _pi, _qi;
                float m = particle->w;
                _pi = particle->p - cm;
                _qi = pair.localX - cmOrigin;

                Eigen::Vector3f pi(_pi.x(), _pi.y(), _pi.z());
                Eigen::Vector3f qi(_qi.x(), _qi.y(), _qi.z());

                Eigen::Matrix3f _tmpAp = pi * qi.transpose();
                Eigen::Matrix3f _tmpAq = qi * qi.transpose();
                Ap += _tmpAp;
                Aq += _tmpAq;
            }
        }
        Eigen::MatrixXf A = Ap * Aq.inverse();
    //    Eigen::MatrixXf A = Aq.inverse() * Ap ;
        Eigen::JacobiSVD<Eigen::MatrixXf> svd(A, Eigen::ComputeThinU | Eigen::ComputeThinV);
        Eigen::MatrixXf UV = svd.matrixU() * svd.matrixV().transpose();
    //    Eigen::MatrixXf UV = svd.matrixV() * svd.matrixU();
        QMatrix3x3 R;

        std::cout<<A<<std::endl;
        std::cout<<"------"<<std::endl;

        for(int i = 0 ;i<9; i++)
        {
            R.data()[i] = UV.data()[i];
        }

        QQuaternion rot =QQuaternion::fromRotationMatrix(R);
        modelMatrix.setToIdentity();
        modelMatrix.rotate(rot);

    //    mlog<<rot;
    //    modelMatrix.translate((cm - cmOrigin));
    //    modelMatrix.rotate(rot);

        count = 0;
        for(auto pair : m_configuration)
        {
            if(auto particle = pair.particle.lock())
            {
                particle->p = modelMatrix * pair.localX;
    //            QVector4D point = QVector4D(pair.localX.x(), pair.localX.y(), pair.localX.z(), 1);

    //            QVector4D test = modelMatrix * point;
    //            mlog<<count<<test<<"  "<< particle->p <<"  "<< particle->x<<"  "<< (cm );
                count++;
            }
        }

    }
//    mlog<<R;

//    std::cout<<UV<<std::endl;
//    mlog<<R;
//    std::cout<<"---------"<<std::endl;

    m_dirty = false;
}

float ShapeMatchingConstraint::constraintFunction()
{
    return 0.0;
}




















