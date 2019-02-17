#include "include/manipulator.h"

#include "model.h"
#include "Scene.h"
#include "GLWidget.h"

Manipulator::Manipulator(Scene* _scene, ModelPtr _vectorModel, QOpenGLShaderProgram* _program)
    : scene(_scene),
      vecotorModel(_vectorModel),
      m_shaderProgram(_program)
{
    localX = QMatrix4x4();
    localX.rotate(-90, QVector3D(0,0,1));

    localZ = QMatrix4x4();
    localZ.rotate(90, QVector3D(1,0,0));

    m_window = scene->widget();
    m_pickingProgram = scene->m_picking_program;
//    m_pickingProgram = scene->m_picking_program;

    m_framebuffer = new Framebuffer();

//    m_Transform.setRotation(QVector3D(15,45,45));
//        m_Transform.setRotation(QVector3D(0,0,45));
//        m_Transform.setRotation(QVector3D(0,45,0));
//    m_Transform.setRotation(QVector3D(15,45,45));

    axisModel = scene->getModelFromPool("Axis");
    circleModel = scene->getModelFromPool("Circle");

    timer.start();

}

void Manipulator::draw()
{

//    m_framebuffer->bind();
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer->m_msfbo);
//    m_shaderProgram->setUniformValue("test",  scene->m_arcCamera.translation());

        m_shaderProgram->bind();

    m_shaderProgram->setUniformValue("ProjectionMatrix", scene->m_projection_matrix);
    m_shaderProgram->setUniformValue("ViewMatrix", scene->m_arcCamera.toMatrix());
    m_shaderProgram->setUniformValue("test",  scene->m_arcCamera.worldPos());

    m_shaderProgram->setUniformValue("ModelMatrix",  m_Transform.toMatrix());
    m_shaderProgram->setUniformValue("color", QVector3D(0,1,0) );
    if(currentState == TRANSLATE_Y)
        m_shaderProgram->setUniformValue("color", QVector3D(1,1,0) );
    vecotorModel->draw();
    m_shaderProgram->setUniformValue("color", QVector3D(0,1,0) );
    if(currentState == ROTATE_Y)
        m_shaderProgram->setUniformValue("color", QVector3D(1,1,0) );
    circleModel->draw();


    m_shaderProgram->setUniformValue("mMaterial.ambient",QVector3D(1,0,0) );
    m_shaderProgram->setUniformValue("color", QVector3D(1,0,0) );
    m_shaderProgram->setUniformValue("ModelMatrix",  m_Transform.toMatrix() * localX);
    if(currentState == TRANSLATE_X)
        m_shaderProgram->setUniformValue("color", QVector3D(1,1,0) );
    vecotorModel->draw();
    m_shaderProgram->setUniformValue("color", QVector3D(1,0,0) );
    if(currentState == ROTATE_X)
        m_shaderProgram->setUniformValue("color", QVector3D(1,1,0) );
    circleModel->draw();


    m_shaderProgram->setUniformValue("mMaterial.ambient",QVector3D(0,0,1) );
    m_shaderProgram->setUniformValue("color", QVector3D(0,0,1) );
    m_shaderProgram->setUniformValue("ModelMatrix",  m_Transform.toMatrix() * localZ);
    if(currentState == TRANSLATE_Z)
        m_shaderProgram->setUniformValue("color", QVector3D(1,1,0) );
    vecotorModel->draw();
    m_shaderProgram->setUniformValue("color", QVector3D(0,0,1) );
    if(currentState == ROTATE_Z)
        m_shaderProgram->setUniformValue("color", QVector3D(1,1,0) );
    circleModel->draw();

}

void Manipulator::drawPickingBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer->m_fbo);
//    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer->m_msfbo);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_pickingProgram->bind();

    m_pickingProgram->setUniformValue("color", QVector3D(0,0,0));
    m_pickingProgram->setUniformValue("ProjectionMatrix", scene->m_projection_matrix);
    m_pickingProgram->setUniformValue("ViewMatrix", scene->m_arcCamera.toMatrix());

    m_pickingProgram->setUniformValue("IDindex",  TRANSLATE_Y);
    m_pickingProgram->setUniformValue("ModelMatrix",  m_Transform.toMatrix());
        vecotorModel->draw();
    m_pickingProgram->setUniformValue("IDindex",  ROTATE_Y);
        circleModel->draw();

    m_pickingProgram->setUniformValue("IDindex",  TRANSLATE_X);
    m_pickingProgram->setUniformValue("ModelMatrix",  m_Transform.toMatrix() * localX);
        vecotorModel->draw();
    m_pickingProgram->setUniformValue("IDindex",  ROTATE_X);
        circleModel->draw();

    m_pickingProgram->setUniformValue("IDindex",  TRANSLATE_Z);
    m_pickingProgram->setUniformValue("ModelMatrix",  m_Transform.toMatrix() * localZ);
        vecotorModel->draw();
    m_pickingProgram->setUniformValue("IDindex",  ROTATE_Z);
        circleModel->draw();
}

void Manipulator::drawPickingBufferDebug()
{
//    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer->m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer->m_msfbo);

    m_pickingProgram->bind();


    m_pickingProgram->setUniformValue("color", QVector3D(1,0,1));

    m_pickingProgram->setUniformValue("ProjectionMatrix", scene->m_projection_matrix);
    m_pickingProgram->setUniformValue("ViewMatrix", scene->m_arcCamera.toMatrix());

    m_pickingProgram->setUniformValue("IDindex",  TRANSLATE_Y);
    m_pickingProgram->setUniformValue("ModelMatrix",  m_Transform.toMatrix());
    vecotorModel->draw();
        m_pickingProgram->setUniformValue("IDindex",  TRANSLATE_X);
    m_pickingProgram->setUniformValue("ModelMatrix",  m_Transform.toMatrix() * localX);
    vecotorModel->draw();
        m_pickingProgram->setUniformValue("IDindex",  TRANSLATE_Z);
    m_pickingProgram->setUniformValue("ModelMatrix",  m_Transform.toMatrix() * localZ);
    vecotorModel->draw();
}


Transform Manipulator::getTransform()
{
    return m_Transform;
}

void Manipulator::setTransform(const Transform &_transform)
{
    m_Transform = _transform;
}

//void Manipulator::setScene(Scene *_scene)
//{
//    scene = _scene;
//    m_pickingProgram = scene->m_picking_program;
//    m_window = scene->widget();
//}

void Manipulator::update()
{
    if(m_window)
    {
//        m_Transform = m_Transform + m_tt;
        if(isDraging)
            return;

        float pixelf[3];
        GLuint pixelui[3];
        GLubyte pixelub[3];

        QPoint screenMouse = m_window->getMouseScreenCoords();

        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_framebuffer->m_fbo);
        glReadBuffer(GL_COLOR_ATTACHMENT0);

        glReadPixels(screenMouse.x(), screenMouse.y(), 1, 1, GL_RGB, GL_FLOAT, &pixelf);
//        glReadPixels(screenMouse.x(), screenMouse.y(), 1, 1, GL_RGB, GL_UNSIGNED_INT, &pixelui);
//        glReadPixels(screenMouse.x(), screenMouse.y(), 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixelub);


//        qDebug()<<"pixelf: "<<pixelf[0]<<pixelf[1]<<pixelf[2]<<uint(pixelf[0]);
//        qDebug()<<"pixelui: "<<pixelui[0]<<pixelui[1]<<pixelui[2];
//        qDebug()<<"pixelub: "<<pixelub[0]<<pixelub[1]<<pixelub[2];

        switch(uint(pixelf[0]))
        {
            case NONE:
                currentState = NONE;
                break;

            case TRANSLATE_X:
                currentState = TRANSLATE_X;
                break;

            case TRANSLATE_Y:
//                qDebug()<<"Y:"<<uint(pixelf[0])<<TRANSLATE_Y;
                currentState = TRANSLATE_Y;
                break;

            case TRANSLATE_Z:
//                qDebug()<<"Z:"<<uint(pixelf[0])<<TRANSLATE_Z;
                currentState = TRANSLATE_Z;
                break;
            case ROTATE_X:
                currentState = ROTATE_X;
                break;
            case ROTATE_Y:
                currentState = ROTATE_Y;
                break;
            case ROTATE_Z:
                currentState = ROTATE_Z;
                break;
        }
    }
}

void Manipulator::startDrag()
{
//    m_startMouseDrag = m_window->getMouseScreenCoords();
    QPoint currentMouse = m_window->getMouseScreenCoords();
    m_startMouseDrag = QPoint(currentMouse.x() - (scene->width() / 2) , currentMouse.y() - (scene->height() / 2));

    QPointF currentMouseNDC = m_window->getMouseNDCCoords();
    Ray camRay = scene->castRayFromCamera(currentMouseNDC.x(), currentMouseNDC.y());
    QQuaternion rot = m_Transform.rotation();
    QVector3D axis = rot.rotatedVector(QVector3D(1,0,0));
    axis.normalize();
    Ray tRay;
    tRay.Origin = m_Transform.translation();
    tRay.Dir = axis.normalized();
    QVector3D closestPoint = scene->m_CollisionDetect.closetPointFromRayToRay(camRay, tRay);

    float d = (closestPoint - m_Transform.translation()).length();
    m_dragStart = m_Transform.translation();
    m_dragStartOffset = m_Transform.translation() + QVector3D(axis.x() *  d, axis.y() * d, axis.z() * d);

//    mlog<<m_dragStart;
//    m_dragStart = m_Transform.translation();
    isDraging = true;
}

void Manipulator::endDrag()
{
    isDraging = false;
}

void Manipulator::drag()
{
    QPoint currentMouse = m_window->getMouseScreenCoords();
    QPointF currentMouseNDC = m_window->getMouseNDCCoords();
    Ray camRay = scene->castRayFromCamera(currentMouseNDC.x(), currentMouseNDC.y());


    QMatrix4x4 mv =  scene->m_projection_matrix *  scene->m_arcCamera.toMatrix();

    QQuaternion rot = m_Transform.rotation();
    QVector3D axis = rot.rotatedVector(QVector3D(1,0,0));
    Ray tRay;
    tRay.Origin = m_Transform.translation();
    tRay.Dir = axis.normalized();

    QVector3D closestPoint = scene->m_CollisionDetect.closetPointFromRayToRay(camRay, tRay);

    QVector3D target = (m_dragStartOffset - closestPoint);
    float rdis = target.length()  ;

    double dot = QVector3D::dotProduct(target, axis);

    float sign = signbit(dot) ? 1 : -1;

    float ds = rdis * sign;

    QPoint currentMouseDrag = QPoint(currentMouse.x() - (scene->width() / 2) , currentMouse.y() - (scene->height() / 2));
    QPoint moveVector = currentMouseDrag - m_startMouseDrag;
    QVector3D moveVec = QVector3D(moveVector.x(), moveVector.y(), 0);

    float l = moveVec.length();
    moveVec.normalize();
    axis.normalize();


    QVector3D axisSS = mv * axis;
    float d = QVector3D::dotProduct(moveVec , axisSS);

    switch(currentState)
    {
        case NONE:
            break;

        case TRANSLATE_X:
            float add = d*l * 0.1;
//            QVector3D t = m_dragStart + QVector3D(axis.x() *  rdis, axis.y() * rdis, axis.z() * rdis) -  m_dragStartOffset;
            QVector3D t = m_dragStart + (axis * ds);
            m_Transform.setTranslation(t);

//            m_Transform.setTranslation(closestPoint);
//            mlog<<closestPoint;

            break;

    }
}
