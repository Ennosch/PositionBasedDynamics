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

    // init data
    for(auto restShape : m_rb->m_restShape)
    {
        if(auto particle = restShape.particle.lock())
        {
            smParticle p;
            p.p = &particle->p;
//            mlog<<"inititlaize          :"<<particle->x;
            p.localRest = restShape.localPos;
            p.target = particle->p;
            cmOrigin += p.localRest;
            m_ShapeParticles.push_back(p);
            m_particles.push_back(particle);
        }
    }
    cmOrigin /= m_ShapeParticles.size();

    std::cout<<"compute Aqq \n"<<std::endl;

    Aq.setIdentity();
    for(auto particle : m_ShapeParticles)
    {
        QVector3D _qi = particle.localRest;
        mlog<<_qi;
        QVector3D qiQt = _qi- cmOrigin;
//        Eigen::Vector3f qi(qiQt.x(), qiQt.y(), qiQt.z());

        Eigen::Vector3f qi(particle.localRest.x() - cmOrigin.x() ,
                           particle.localRest.y() - cmOrigin.y() ,
                           particle.localRest.z() - cmOrigin.z() );

        std::cout<<qi<<std::endl;
        std::cout<<"------"<<std::endl;

        Aq += qi * qi.transpose();
    }
//    mlog<<"-\n end init-0-------\n\n";


    std::cout<<"this is Aq"<<std::endl;
    std::cout<<Aq<<std::endl;
//    mlog<<"++==++++=+===================";
}

void ShapeMatchingConstraint::projectOld()
{
    if(!m_dirty)
        return;

    // prepare data
    // cache target position
    for(auto sParticle : m_ShapeParticles)
    {
        sParticle.target = *sParticle.p;
        cmTarget += sParticle.target;
    }
    cmTarget /= m_ShapeParticles.size();


    for(int j=0; j<1;j ++)
    {
        // CM
        for(auto sParticle : m_ShapeParticles)
        {
            cm += *sParticle.p;
        }
        cm /= m_ShapeParticles.size();

        Aq.setIdentity();
        Ap.setIdentity();
        for(auto sParticle : m_ShapeParticles)
        {
            Eigen::Vector3f _qi(sParticle.p->x(), sParticle.p->y(), sParticle.p->z());
            Eigen::Vector3f qi = _qi - Eigen::Vector3f(cm.x() , cm.y() ,cm.z());
            Aq += qi * qi.transpose();

            float test = sParticle.target.x();
            Eigen::Vector3f _pi(sParticle.target.x(), sParticle.target.y(), sParticle.target.z());
            Eigen::Vector3f pi = _pi - Eigen::Vector3f(cmTarget.x() , cmTarget.y() ,cmTarget.z());
            Ap += pi * qi.transpose();
        }

        Eigen::Matrix3f A = Ap * Aq.inverse();
        Eigen::JacobiSVD<Eigen::MatrixXf> svd(A, Eigen::ComputeThinU | Eigen::ComputeThinV);

        Eigen::MatrixXf UV = svd.matrixU() * svd.matrixV().transpose();
        QMatrix3x3 R;

        for(int i = 0 ;i<9; i++){
            R.data()[i] = UV.data()[i];
        }

        QMatrix4x4 transform;
        transform.setToIdentity();
        QQuaternion rot = QQuaternion::fromRotationMatrix(R);
        transform.translate(cmTarget - cm);
//        transform.rotate(rot);

        QVector3D eulerAngles = rot.toEulerAngles();
        mlog<<R;

        for(auto sParticle : m_ShapeParticles)
        {
            QVector3D p = *sParticle.p;
//            QVector3D corret = QVector3D(1,0,0);
//            *sParticle.p = corret;
        }
    }
    mlog<<"update";
    m_dirty = false;
}

void ShapeMatchingConstraint::project1D()
{
    if(!m_dirty)
        return;

    cm = QVector3D(0,0,0);
    for(auto particle : m_particles)
    {
        cm += particle->p;
    }
    cm /= m_particles.size();

    Ap.setIdentity();
    for(auto particle : m_ShapeParticles)
    {
        QVector3D _qi = particle.localRest;
        QVector3D qiQt = _qi- cmOrigin;
        Eigen::Vector3f qi(qiQt.x(), qiQt.y(), qiQt.z());

        QVector3D _pi = *particle.p;
        QVector3D piQt = _pi- cm;
        Eigen::Vector3f pi(piQt.x(), piQt.y(), piQt.z());

//        std::cout<<pi<<std::endl;
//        std::cout<<"---------"<<std::endl;

        Ap += pi * qi.transpose();
    }

//    std::cout<<Ap<<std::endl;

    Eigen::Matrix3f A = Ap * Aq.inverse();
    Eigen::JacobiSVD<Eigen::MatrixXf> svd(A, Eigen::ComputeThinU | Eigen::ComputeThinV);

    Matrix3r Apolar = A;
    Matrix3r Rpolar = A;

    polarDecompositionStable(Apolar, eps, Rpolar);

//    std::cout<<"Rpolar: -----------------"<<std::endl;
//    std::cout<<Rpolar<<std::endl;

    Eigen::MatrixXf UV = svd.matrixU() * svd.matrixV().transpose();
    QMatrix3x3 R;

//    std::cout<<"SVD: -----------------"<<std::endl;
//    std::cout<<UV<<std::endl;

    // copy Eigen3x3 rotation matrix into QMatrix3x3
    for(int i = 0 ;i<9; i++){
        R.data()[i] = UV.data()[i];
    }

    QMatrix4x4 transform;
    transform.setToIdentity();
    QQuaternion rot = QQuaternion::fromRotationMatrix(R);
    QVector3D eulerAngles = rot.toEulerAngles();
    transform.translate(cm - cmOrigin);
    transform.rotate(rot);

    for(auto particle : m_particles)
    {
//        particle->p = transform
//        mlog<<"x: "<<particle->x;
    }

//    mlog<<transform;
    for(auto shapeParticle : m_ShapeParticles)
    {
        QVector3D p = *shapeParticle.p;
        QVector3D dp = transform * shapeParticle.localRest;
        *shapeParticle.p = dp;

//        QVector3D goal = cm + (R * (shapeParticle.localRest -cmOrigin));

        Eigen::Vector3f x0(shapeParticle.localRest.x(), shapeParticle.localRest.y(), shapeParticle.localRest.z());
        Eigen::Vector3f restCm(cmOrigin.x(), cmOrigin.y(), cmOrigin.z());
        Eigen::Vector3f x(p.x(), p.y(), p.z());
        Eigen::Vector3f cme(cm.x(), cm.y(), cm.z());

        Eigen::Vector3f goal = cme + UV * (x0 - restCm);
        Eigen::Vector3f corr = (goal - x);

        Eigen::Vector3f eigenResult = x + corr;

//        mlog<<"p:   "<<p<<"dp: "<<dp;


//        mlog<<"eigenResult:     "<<eigenResult.x()<<"   "<<eigenResult.y()<<"   "<<eigenResult.z()<<"           QResult: "<<dp.x()<<"       "<<dp.y()<<"       "<<dp.z()   ;

    }

//    for (int i = 0; i < numPoints; i++) {
//		Vector3r goal = cm + R * (x0[i] - restCm);
//		corr[i] = (goal - x[i]) * stiffness;
//	}

//    mlog<<"euler Angles"<<eulerAngles;
//    mlog<<"step";

    m_dirty = false;
}

void ShapeMatchingConstraint::project()
{
    project1D();
//    if(!m_dirty)
//        return;

//    int numIterations = 1;
//    for(int j=0; j<numIterations;j ++)
//    {

//        // compute cm
//        // compute Ap
//        // A (Ap Aq) and rot

//        //
//        // Particle->p = transform * localP;


//    }

//    m_dirty = false;
}

float ShapeMatchingConstraint::constraintFunction()
{
    return 0.0;
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










