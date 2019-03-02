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
class ActiveObject;


enum State{
    NONE,
    TRANSLATE_X,
    TRANSLATE_Y,
    TRANSLATE_Z,
    TRANSLATE_YZ,
    TRANSLATE_XZ,
    TRANSLATE_XY,
    TRANSLATE_VIEWPLANE,
    ROTATE_X,
    ROTATE_Y,
    ROTATE_Z,
    ROTATE_VIEWPLANE
};

class Manipulator
{
public:
    Manipulator(Scene* _scene, ModelPtr _vectorModel, QOpenGLShaderProgram* _program);
    void draw();
    void drawPickingBuffer();

    Transform getTransform();
    void setTransform(const Transform &_transform);
    void setRotation(const QQuaternion &_rot);
    void setTranslation(const QVector3D &_translation);

    void update();
    void updateLocalView();
    void updateGlobalScale();
    void startDrag();
    void endDrag();
    void drag();
    void dragRotate();
    void dragTranslate();
    void dragTranslateViewPlane();

//private:
    friend Scene;
    bool isDraging;
    bool worldSpace;

    ModelPtr vecotorModel;
    ModelPtr axisModel;
    ModelPtr circleModel;
    ModelPtr planeModel;
    Transform m_Transform;
    QMatrix4x4 localZ, localX, localView;
    QMatrix4x4 globalScale;

    QOpenGLShaderProgram* m_shaderProgram;
    QOpenGLShaderProgram* m_pickingProgram;
    QPoint m_startMouseDrag;
    Framebuffer* m_framebuffer;
    Scene* scene;
    GLWidget *m_window = nullptr;

    State currentState = NONE;
    QVector3D m_dragStart, m_dragStartOffset;
    QVector3D m_dragStartRotVec;
    QQuaternion m_dragStartRot;
    QVector3D localAxis;
    QVector3D localUp, localRight;

    ActiveObject *m_activeObject;

};

#endif // MANIPULATOR_H
