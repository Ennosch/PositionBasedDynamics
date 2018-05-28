#include "camera.h"
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

void Camera3D::rotateAroundPoint(float _xoffset, float _yoffset)
{
    m_world.translate(-m_pivot);
    QQuaternion rotq = QQuaternion::fromAxisAndAngle(0, 1, 0, _xoffset);
    m_world.rotate(rotq);

    rotq = QQuaternion::fromAxisAndAngle( -m_right.x(),
                                          -m_right.y(),
                                          -m_right.z(),
                                          _yoffset);
    m_world.rotate(rotq);
    m_world.translate(m_pivot);
    //rotq=createFromAngle(0,1,0,_xoffset);
    // Update camera orientation vectors
    m_right.setX(-m_world(0,0));
    m_right.setY(-m_world(0,1));
    m_right.setZ(-m_world(0,2));

    m_up.setX(-m_world(1,0));
    m_up.setY(-m_world(1,1));
    m_up.setZ(-m_world(1,2));

    m_front.setX(-m_world(2,0));
    m_front.setY(-m_world(2,1));
    m_front.setZ(-m_world(2,2));
}

void Camera3D::rotateAroundPoint_B(float _xoffset, float _yoffset)
{
    m_world.translate(-m_pivot);
    QQuaternion rotX= QQuaternion::fromAxisAndAngle(0, 1, 0, _xoffset);
    //forward(), QVector3D (0,1,0)

    //QVector3D _right = QVector3D::crossProduct(forward(), QVector3D (0,1,0));
    //qDebug()<<_right<<right();





    QQuaternion rotY = (QQuaternion::fromAxisAndAngle(right(), _yoffset));
    //QQuaternion rotq = QQuaternion::fromAxisAndAngle(0, 1, 0, _xoffset);
    //m_rotation = dr * m_rotation;
    //m_dirty = true;

    m_world.rotate(rotY);
    m_world.rotate(rotX);
    m_world.translate(-m_pivot);
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
    qDebug("ininin");
    m_dirty = false;
    m_world.setToIdentity();
    //qDebug()<<m_rotation;
   m_world.rotate(m_rotation.conjugate());
   m_world.translate(-m_translation);
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
  return m_rotation.rotatedVector(LocalRight);
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

