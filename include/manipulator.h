#ifndef MANIPULATOR_H
#define MANIPULATOR_H

#include <QMatrix4x4>
#include <QOpenGLShaderProgram>

#include "utils.h"
#include "Framebuffer.h"
#include "transform.h"

class Scene;
class GLWidget;

enum State{
    TRANSLATE_X,
    TRANSLATE_Y,
    TRANSLATE_Z
};

class Manipulator
{
public:
    Manipulator(Scene* _scene, ModelPtr _vectorModel, QOpenGLShaderProgram* _program);
//    Manipulator();
    void draw();
    void drawPickingBuffer();

    Transform getTransform();
    void setTransform(const Transform &_transform);
//    void setScene(Scene*_scene);
    void update();

    int* var;

//private:
    friend Scene;
    ModelPtr vecotorModel;
    Transform m_Transform;
    QMatrix4x4 localZ, localX;
    QOpenGLShaderProgram* m_shaderProgram;
    QOpenGLShaderProgram* m_pickingProgram;
    Framebuffer* m_framebuffer;
    Scene* scene;
    GLWidget *m_window = nullptr;
    int* test =  nullptr;
};

#endif // MANIPULATOR_H
