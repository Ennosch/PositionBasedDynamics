#ifndef ACTIVEOBJECT_H
#define ACTIVEOBJECT_H

#include <QObject>
#include <QMatrix4x4>
#include <QVector3D>

#include "utils.h"
#include "sceneobject.h"

class GLWidget;

class ActiveObject : public QObject
{
    Q_OBJECT

public:
    ActiveObject();
    ActiveObject(GLWidget *_widget);
    void notify(int _id);
    void notify(pSceneOb _sender);
    void notify(const Transform &_t);
    void setTransform(const QVector3D _t, const QVector3D _r, const QVector3D _s);

signals:
    void transformChanged(const QMatrix4x4 _modelMat, const QVector3D num, const QVector3D num1, const QVector3D num2);
    void testSignal(int p);

private:
    pSceneOb activeSceneObject;
    GLWidget *m_GLWidget;
};

#endif // ACTIVEOBJECT_H
