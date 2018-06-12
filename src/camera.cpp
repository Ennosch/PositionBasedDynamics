#include "camera.h"
#include <cmath>
#include <QDebug>


const QVector3D Camera3D::LocalForward(0.0f, 0.0f, -1.0f);
const QVector3D Camera3D::LocalUp(0.0f, 1.0f, 0.0f);
const QVector3D Camera3D::LocalRight(1.0f, 0.0f, 0.0f);

const float m_pi = 3.14159265;

// Transform By (Add/Scale)
void Camera3D::translate(const QVector3D &dt)
{
  m_dirty = true;
  m_translation += dt;
}

void Camera3D::rotate(const QQuaternion &dr)
{
  m_dirty = true;
  m_rotation = dr * m_rotation;
}

void Camera3D::rotateAroundPoint(const float _angle, const QVector3D &_axis)
{
    QQuaternion _rot= QQuaternion::fromAxisAndAngle(_axis, _angle);
    //manipulate the matrix directly is possible but dont
    //m_world.rotate(_rot);
    m_rotation = m_rotation * _rot;

    m_dirty = true;
    toMatrix();

    // retrive camera world position from m_world 4x4 Matrix
    QVector4D d(m_world.column(3));
    QVector4D retVec = -d * m_world;
    QVector3D _worldPos = QVector3D(retVec.x(), retVec.y(), retVec.z());
    m_worldPos = _worldPos;
    m_front = -m_worldPos.normalized();
//    float _dot = QVector3D::dotProduct(-m_worldPos.normalized(), m_worldUp);
    
//    if ( 1 - std::abs(_dot) < 0.1 )
//    {
//        qDebug()<<"critical crossProduct camForward x worldUp cause vector align :( "<<std::abs(_dot) << "Gramm Schmidt kicking in ";
        QVector3D _right = QVector3D::crossProduct(-m_worldPos, QVector3D(1,0,0));
        //qDebug()<<"newRight: "<<_right<<" oldRight: "<<m_right;

        // Gramm Schmidt orthogonlise local m_right, m_forwad, m_up
        QVector3D x1, x2, x3, v1, v2, v3;
        x1 = m_front;
        x2 = m_right;
        x3 = m_up;

        v1 = x1;
        v2 = x2 - ((QVector3D::dotProduct(x2,v1) / QVector3D::dotProduct(v1,v1)) * v1);

        v3 = x3 - ((QVector3D::dotProduct(x3,v1) / QVector3D::dotProduct(v1,v1)) * v1)
                - ((QVector3D::dotProduct(x3,v2) / QVector3D::dotProduct(v2,v2)) * v2);

        m_right = v2;
        m_worldUp *= -1;
//    }
//    else
//    {
//        // if not critial compute normal local right vector with cross product
//        m_right = QVector3D::crossProduct(-m_worldPos.normalized(), QVector3D(0,1,0));
//        m_up = QVector3D::crossProduct(m_front, m_right);
//    }

}

void Camera3D::rotateArcBall(const QPoint _mousePos, const QPoint _mouseTriggeredPos, const int _radius)
{

    //https://pixeladventuresweb.wordpress.com/2016/10/04/arcball-controller/
    //qDebug()<<_mousePos<<_mouseTriggeredPos;



    QVector3D P1, P2, _axis;
    QQuaternion _rot;


    float scale = 1;
    float _x2 = std::abs(_mousePos.x());
    float _y2 = std::abs(_mousePos.y());
    float _x1 = std::abs(_mouseTriggeredPos.x());
    float _y1 = std::abs(_mouseTriggeredPos.y());
    float _z1, _z2, _angle, _dot;

    float _tempRadius = _radius;

    float _rTest1 = std::sqrt(std::pow(_x1,2) + std::pow(_y1, 2));
    float _rTest2 = std::sqrt(std::pow(_x2,2) + std::pow(_y2, 2));

    if(_rTest2 <= _tempRadius)
    {
        _z2 = std::sqrt(std::pow(_tempRadius,2) - std::pow(_x2,2) - std::pow(_y2,2));
    }
    else
    {
        _z2 = 0.0;
    }
    if(_rTest1 <= _tempRadius)
    {
        _z1 = std::sqrt(std::pow(_tempRadius,2) - std::pow(_x1,2) - std::pow(_y1,2));
    }
    else
    {
        _z1 = 0.0;
    }

    P1 = QVector3D(_mouseTriggeredPos.x(), _mouseTriggeredPos.y(), _z1);
    //P2 = QVector3D(_x2,_y2,_z2);
    P2 = QVector3D(_mousePos.x(), _mousePos.y(), _z2);
    _axis = QVector3D::crossProduct(P1,P2);
    _dot = QVector3D::dotProduct(P1.normalized(),P2.normalized());
    _angle = std::acos(_dot) * (180 / m_pi);
    _rot = QQuaternion::fromAxisAndAngle(_axis, _angle);

    m_rotation = _rot * m_startRotation;
    m_dirty = true;

    //qDebug()<<m_rotation<<_length;


}



QQuaternion Camera3D::lookAt()
{
    //-------------WIP------------------
    QVector3D _dir = -m_worldPos;
    QVector3D _aim = QVector3D(0,0,1);

    QQuaternion _rot1, _rot2, _rot3;
    _rot1 = QQuaternion::rotationTo(_aim, _dir);

    //_rot2.fromDirection(_dir, QVector3D(0,1,0));

    QVector3D _right, _up, _newUp, _test;
    _right = QVector3D::crossProduct(_dir, QVector3D(0,1,0));
    _up = QVector3D::crossProduct(_right, _dir);

    _newUp = _rot1 * QVector3D(0.0f,1.0f,0.0f);
    //_test = m_rotation * QVector3D(0.0f ,0.0f ,1.0f);
    _test = _rot1.toEulerAngles();

    _rot2 = QQuaternion::rotationTo(_newUp, _up);

    _rot3 = _rot2 * _rot1;

    //qDebug()<<_test<<_rot1;

//    //apply the correction up rotation
//    m_rotation = m_rotation * _rot3;
//    m_dirty = true;
//    toMatrix();
//    m_worldPos = -m_translation * m_world;

    return _rot3;


}

void Camera3D::reset()
{

//    m_world.setToIdentity();
//    m_world.translate(-m_translation);
//    m_world.lookAt(-m_worldPos, QVector3D(0,0,0),  QVector3D(0,1,0) );

}


// Transform To (Setters)
void Camera3D::setTranslation(const QVector3D &t)
{
  m_dirty = true;
  m_translation = t;
}

void Camera3D::setRotation(const QQuaternion &r)
{
  m_dirty = true;
  m_rotation = r;
}

void Camera3D::setStartRotation()
{
    m_startRotation = m_rotation;
}

// Accessors
const QMatrix4x4 &Camera3D::toMatrix()
{
  if (m_dirty)
  {
    m_dirty = false;
    m_world.setToIdentity();
    //if were to rotate about the cameras center call rot.conjugate before translate
    //m_world.rotate(m_rotation.conjugate());
    m_world.translate(-m_translation);

    m_world.rotate(m_rotation);
  }
  return m_world;
}

// Queries
QVector3D Camera3D::forward() const
{
  return m_rotation.rotatedVector(LocalForward);
}

QVector3D Camera3D::up() const
{
  return m_rotation.rotatedVector(LocalUp);
}

QVector3D Camera3D::right() const
{
  //return m_rotation.rotatedVector(LocalRight);
    return m_right;
}

// Qt Streams
QDebug operator<<(QDebug dbg, const Camera3D &transform)
{
  dbg << "Camera3D\n{\n";
  dbg << "Position: <" << transform.translation().x() << ", " << transform.translation().y() << ", " << transform.translation().z() << ">\n";
  dbg << "Rotation: <" << transform.rotation().x() << ", " << transform.rotation().y() << ", " << transform.rotation().z() << " | " << transform.rotation().scalar() << ">\n}";
  return dbg;
}

QDataStream &operator<<(QDataStream &out, const Camera3D &transform)
{
  out << transform.m_translation;
  out << transform.m_rotation;
  return out;
}

QDataStream &operator>>(QDataStream &in, Camera3D &transform)
{
  in >> transform.m_translation;
  in >> transform.m_rotation;
  transform.m_dirty = true;
  return in;
}

