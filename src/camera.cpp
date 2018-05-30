#include "camera.h"
#include <cmath>
#include <QDebug>


const QVector3D Camera3D::LocalForward(0.0f, 0.0f, -1.0f);
const QVector3D Camera3D::LocalUp(0.0f, 1.0f, 0.0f);
const QVector3D Camera3D::LocalRight(1.0f, 0.0f, 0.0f);

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

void Camera3D::rotateAroundPoint(float _angle, const QVector3D &_axis)
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

    m_right = QVector3D::crossProduct(-m_worldPos.normalized(), QVector3D(0,1,0));

    float _dot = QVector3D::dotProduct(-m_worldPos.normalized(), QVector3D(0,1,0));
    //qDebug()<<_dot;
    //qDebug()<<std::abs(_dot);

    if ( 1 - std::abs(_dot) < 0.01 )
    {
        qDebug()<<"critical crossProduct camForward x worldUp cause vector align :( "<<std::abs(_dot);
        m_right = QVector3D::crossProduct(-m_worldPos, QVector3D(0,1,0));
    }



//    QMatrix4x4 _world = m_world;
//    //m_world.lookAt(m_worldPos, QVector3D(0,0,0),  QVector3D(0,1,0) );

//    QVector4D _d = -m_translation * m_world;


    //qDebug()<<newRightTest_1;
//    qDebug()<<d;
//    qDebug()<<m_world;
//    qDebug()<<_d;
    //qDebug()<<m_world;

}

void Camera3D::lookAt()
{
    //m_dirty = true;
    //toMatrix();
    //qDebug()<<m_worldPos;
    //m_world.lookAt(m_worldPos, QVector3D(0,0,0), QVector3D(0,1,0));
}

QQuaternion Camera3D::lookAt(int _a)
{
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
    m_world.lookAt(-m_worldPos, QVector3D(0,0,0),  QVector3D(0,1,0) );

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

