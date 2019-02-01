#ifndef MANIPULATOR_H
#define MANIPULATOR_H

#include <QMatrix4x4>
#include <QOpenGLShaderProgram>

#include "utils.h"
#include "transform.h"
class Scene;

class Manipulator
{
public:
    Manipulator(ModelPtr _vectorModel, QOpenGLShaderProgram* _program);
    void draw();
    void drawPickingBuffer();

    Transform getTransform();
    void setTransform(const Transform &_transform);
    void setScene(Scene*_scene);

private:
//    friend Scene;
    ModelPtr vecotorModel;
    Transform m_Transform;
    QMatrix4x4 localZ, localX;
    QOpenGLShaderProgram* m_shaderProgram;
    QOpenGLShaderProgram* m_pickingProgram;
    Scene* scene;
};

#endif // MANIPULATOR_H
