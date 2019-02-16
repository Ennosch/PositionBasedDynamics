#ifndef MANIPULATOR_H
#define MANIPULATOR_H

#include <QMatrix4x4>
#include <QOpenGLShaderProgram>

#include <QElapsedTimer>
#include "utils.h"
#include "Framebuffer.h"
#include "transform.h"

class Scene;
class GLWidget;



enum State{
    NONE,
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
    void startDrag();
    void endDrag();
    void drag();


    bool flip;

//private:
    friend Scene;
    bool isDraging;
    ModelPtr vecotorModel;
    Transform m_Transform;
    QMatrix4x4 localZ, localX;
    QOpenGLShaderProgram* m_shaderProgram;
    QOpenGLShaderProgram* m_pickingProgram;
    QPoint m_startMouseDrag;
    Framebuffer* m_framebuffer;
    Scene* scene;
    GLWidget *m_window = nullptr;
    int* test =  nullptr;
    State currentState = NONE;

    QVector3D m_drag, m_dragStart;
    float logX, logXd;
    debug x, xd;

    QElapsedTimer timer;
    Transform m_tt;
};

#endif // MANIPULATOR_H
