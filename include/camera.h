#ifndef QCAMERA_H
#define QCAMERA_H

#include <QPoint>
#include "transform.h"
#include "utils.h"


class Camera3D
{
public:

  // Constants
  static const QVector3D LocalForward;
  static const QVector3D LocalUp;
  static const QVector3D LocalRight;

  // Constructors
  Camera3D();

  // Transform By (Add/Scale)
  void translate(const QVector3D &dt);
  void translatePivot(const QVector3D &dt);
  void translate(float dx, float dy, float dz);
  void rotate(const QQuaternion &dr);
  void rotate(float angle, const QVector3D &axis);
  void rotate(float angle, float ax, float ay, float az);

  void rotateAroundPoint(const float _angle, const QVector3D &_axis);
  void rotateArcBall(const QPoint _mousePos, const QPoint _mouseTriggeredPos, const int _radius); //--------WIP--------
  void arcBallStart();

  void dolly(const QPoint &_mousePos);
  void dollyStart(const QPoint &_mousePos);

  void track(const QPoint &_mousePos);
  void trackStart(const QPoint &_mousePos);
  void info();

  QQuaternion lookAt();// -----WIP------
  void reset(const QPoint &_mousePos);

  // Transform To (Setters)
  void setTranslation(const QVector3D &t);
  void setTranslation(float x, float y, float z);
  void setRotation(const QQuaternion &r);
  void setRotation(float angle, const QVector3D &axis);
  void setRotation(float angle, float ax, float ay, float az);
  void SetPivot(const QVector3D &_pivot);
  void SetWorldPos(const QVector3D &_pos);
  void SetPivotToCam(const QVector3D &_pivot);


  void inline foo(){qDebug("foo");}

  // Accessors
  const QVector3D& translation() const;
  const QQuaternion& rotation() const;
//  const QMatrix4x4& rotationMat4() const;
  const QMatrix4x4& toMatrix();



  // Queries
  QVector3D forward() const;
  QVector3D up() const;
  QVector3D right() const;
  QVector3D worldPos() const;
  QVector3D pivot() const;

private:
  bool m_dirty;

  QVector3D m_translation,
            m_worldPos,
            m_PreWorldPos,
            m_pivot,
            m_pivotToCam,
            m_PrePivotToCam;

  QPoint m_mousePos, m_prevMousePos;

  QQuaternion m_rotation, m_deltaRotation, m_startRotation;

  QMatrix4x4 m_world;

#ifndef QT_NO_DATASTREAM
  friend QDataStream &operator<<(QDataStream &out, const Camera3D &transform);
  friend QDataStream &operator>>(QDataStream &in, Camera3D &transform);
#endif
};

Q_DECLARE_TYPEINFO(Camera3D, Q_MOVABLE_TYPE);


inline void Camera3D::info()
{
    // rotate 90 deg around the y-axis (clockwise)
    QQuaternion myQuat = QQuaternion(0.707, 0, 0.707, 0);
    QVector3D myVec = QVector3D(0, 0, 6);
    QVector3D r = myQuat.rotatedVector(myVec);

    QMatrix3x3 tmp = m_rotation.toRotationMatrix();
    float *m = tmp.data();
        const float data[16]  = {
                m[0],m[1],m[2],0,
                m[3],m[4],m[5],0,
                m[6],m[7],m[8],0,
                   0, 0, 0, 1
        };
    const float *ptr = data;
    QMatrix4x4 myRotMat = QMatrix4x4(ptr);

// only works in 1 axis ?
//    m_worldPos = -(m_rotation.rotatedVector(-m_translation));
//    m_worldPos = myRotMat * m_translation;
     m_worldPos = myRotMat * (m_translation - m_pivot) + m_pivot;

    myqDebug()<<"WP: "<<m_worldPos<<" translation :"<<m_translation<< "pivot: "<<m_pivot<<" rot: "<<m_rotation;

};

inline QVector3D Camera3D::pivot() const { return m_pivot; };
inline void Camera3D::SetPivotToCam(const QVector3D &_pivot) { m_pivotToCam = _pivot; };

// Constructors

// Transform By (Add/Scale)
inline void Camera3D::translate(float dx, float dy,float dz) { translate(QVector3D(dx, dy, dz)); }
inline void Camera3D::translatePivot(const QVector3D &dt) { m_pivot += dt; };
inline void Camera3D::rotate(float angle, const QVector3D &axis) { rotate(QQuaternion::fromAxisAndAngle(axis, angle)); }
inline void Camera3D::rotate(float angle, float ax, float ay,float az) { rotate(QQuaternion::fromAxisAndAngle(ax, ay, az, angle)); }

// Transform To (Setters)
inline void Camera3D::setTranslation(float x, float y, float z) { setTranslation(QVector3D(x, y, z)); }
inline void Camera3D::setRotation(float angle, const QVector3D &axis) { setRotation(QQuaternion::fromAxisAndAngle(axis, angle)); }
inline void Camera3D::setRotation(float angle, float ax, float ay, float az) { setRotation(QQuaternion::fromAxisAndAngle(ax, ay, az, angle)); }
inline void Camera3D::SetPivot(const QVector3D &_pivot){ m_pivot = _pivot; };

inline void Camera3D::SetWorldPos(const QVector3D &_pos){ m_worldPos = _pos; };

// Accessors
inline const QVector3D& Camera3D::translation() const { return m_translation; }
inline const QQuaternion& Camera3D::rotation() const { return m_rotation; }

// Qt Streams
#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const Camera3D &transform);
#endif

#ifndef QT_NO_DATASTREAM
QDataStream &operator<<(QDataStream &out, const Camera3D &transform);
QDataStream &operator>>(QDataStream &in, Camera3D &transform);
#endif

#endif // QCAMERA_H
