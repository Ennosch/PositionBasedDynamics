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

    m_Transform.setRotation(QVector3D(15,45,45));
//    m_Transform.setRotation(QVector3D(15,45,45));

    timer.start();

}

void Manipulator::draw()
{

//        m_framebuffer->bind();
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer->m_msfbo);
//    m_shaderProgram->setUniformValue("test",  scene->m_arcCamera.translation());

        m_shaderProgram->bind();

    m_shaderProgram->setUniformValue("ProjectionMatrix", scene->m_projection_matrix);
    m_shaderProgram->setUniformValue("ViewMatrix", scene->m_arcCamera.toMatrix());
    m_shaderProgram->setUniformValue("test",  scene->m_arcCamera.worldPos());

    m_shaderProgram->setUniformValue("ModelMatrix",  m_Transform.toMatrix());
    m_shaderProgram->setUniformValue("mMaterial.shininess", 25 );
    m_shaderProgram->setUniformValue("mMaterial.specular", QVector3D(0,0,0) );
    m_shaderProgram->setUniformValue("mMaterial.ambient",QVector3D(0,1,0) );
    m_shaderProgram->setUniformValue("mMaterial.diffuse", QVector3D(0,1,0) );
    if(currentState == TRANSLATE_Y)
        m_shaderProgram->setUniformValue("mMaterial.diffuse", QVector3D(1,1,0) );
    vecotorModel->draw();
    m_shaderProgram->setUniformValue("mMaterial.ambient",QVector3D(1,0,0) );
    m_shaderProgram->setUniformValue("mMaterial.diffuse", QVector3D(1,0,0) );
    m_shaderProgram->setUniformValue("ModelMatrix",  m_Transform.toMatrix() * localX);
    if(currentState == TRANSLATE_X)
        m_shaderProgram->setUniformValue("mMaterial.diffuse", QVector3D(1,1,0) );
    vecotorModel->draw();
    m_shaderProgram->setUniformValue("mMaterial.ambient",QVector3D(0,0,1) );
    m_shaderProgram->setUniformValue("mMaterial.diffuse", QVector3D(0,0,1) );
    m_shaderProgram->setUniformValue("ModelMatrix",  m_Transform.toMatrix() * localZ);
    if(currentState == TRANSLATE_Z)
        m_shaderProgram->setUniformValue("mMaterial.diffuse", QVector3D(1,1,0) );
    vecotorModel->draw();
}

void Manipulator::drawPickingBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer->m_fbo);
    m_pickingProgram->bind();

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


//        qDebug()<<"pixelf: "<<pixelf[0]<<pixelf[1]<<pixelf[2]<<uint(pixelf[0])<<TRANSLATE_X;
//        qDebug()<<"pixelui: "<<pixelui[0]<<pixelui[1]<<pixelui[2];
//        qDebug()<<"pixelub: "<<pixelub[0]<<pixelub[1]<<pixelub[2];

        switch(uint(pixelf[0]))
        {
            case NONE:
                currentState = NONE;
                break;

            case TRANSLATE_X:
//                qDebug()<<"X:"<<uint(pixelf[0])<<TRANSLATE_X;
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
        }

    }
}

void Manipulator::startDrag()
{
//    m_startMouseDrag = m_window->getMouseScreenCoords();
    QPoint currentMouse = m_window->getMouseScreenCoords();
    m_startMouseDrag = QPoint(currentMouse.x() - (scene->width() / 2) , currentMouse.y() - (scene->height() / 2));

    m_dragStart = m_Transform.translation();
    isDraging = true;
}

void Manipulator::endDrag()
{
    isDraging = false;
}

void Manipulator::drag()
{
    QPoint currentMouse = m_window->getMouseScreenCoords();
    QVector3D localX = QVector3D(1,0,0);
    QMatrix4x4 mvp = scene->m_projection_matrix *  scene->m_arcCamera.toMatrix() * m_Transform.toMatrix();
    QMatrix4x4 mv =  scene->m_projection_matrix *  scene->m_arcCamera.toMatrix();
    QVector3D ndcX = mvp * localX;
    QVector3D axis =  QVector3D(ndcX.x() * scene->width(), ndcX.y() * scene->height(), 0 );
    axis = QVector3D(1,0,0);

    QQuaternion rot = m_Transform.rotation();
    axis = rot.rotatedVector(QVector3D(1,0,0));

//    mlog<<axis<<ndcX<<localAxis;

    QPoint currentMouseDrag = QPoint(currentMouse.x() - (scene->width() / 2) , currentMouse.y() - (scene->height() / 2));

    QPoint moveVector = currentMouseDrag - m_startMouseDrag;

    QVector3D moveVec = QVector3D(moveVector.x(), moveVector.y(), 0);

    float l = moveVec.length();
    moveVec.normalize();
    axis.normalize();

    QVector3D axisDot = QVector3D(axis.x(), axis.y(), 0);

    QVector3D axisSS = mv * axis;

    float d = QVector3D::dotProduct(moveVec , axisSS);




    switch(currentState)
    {
        case NONE:

//            qDebug()<<"setting";

            break;

        case TRANSLATE_X:
            mlog<<moveVec<<axis;
//            qDebug()<<axis<<moveVector<<"d: "<<d<<"l:"<<l<<d*l;
//              qDebug()<<d*l * 0.01;

            x.move = moveVec;
            x.axis = axis;
            x.d =  d;
            x.l = l;
            x.xt = d * l * 0.01;
            x.startD = m_dragStart;
            x.addD = QVector3D(d*l * 0.01,0,0);

//            QVector3D t = m_dragStart + QVector3D(d*l * 0.01,0,0);

            float add = d*l * 0.01;
            QVector3D t = m_dragStart + QVector3D(axis.x() *  add, axis.y() * add, axis.z() * add);

            if((x.xt > 0 && xd.xt < 0) || (x.xt < 0 && xd.xt > 0) )
            {
                if (flip == true)
                {
//                    qDebug()<<"flipp"<<x.xt<<xd.xt<<timer.elapsed();
                }
                flip = true;
            }
            else
            {
                flip = false;
            }

            m_Transform.Debug = x;
            m_Transform.setTranslation(t);

            xd = x;

            break;

//        case TRANSLATE_Y:

//            break;

//        case TRANSLATE_Z:

//            break;
    }
}
