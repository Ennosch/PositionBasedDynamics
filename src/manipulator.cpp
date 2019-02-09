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

}

void Manipulator::draw()
{
//    m_shaderProgram->setUniformValue("test",  scene->m_arcCamera.translation());
    m_shaderProgram->setUniformValue("test",  scene->m_arcCamera.worldPos());

    m_shaderProgram->setUniformValue("ModelMatrix",  m_Transform.toMatrix());
    m_shaderProgram->setUniformValue("mMaterial.shininess", 25 );
    m_shaderProgram->setUniformValue("mMaterial.specular", QVector3D(0,0,0) );
    m_shaderProgram->setUniformValue("mMaterial.ambient",QVector3D(0,1,0) );
    m_shaderProgram->setUniformValue("mMaterial.diffuse", QVector3D(0,1,0) );
    vecotorModel->draw();
    m_shaderProgram->setUniformValue("mMaterial.ambient",QVector3D(1,0,0) );
    m_shaderProgram->setUniformValue("mMaterial.diffuse", QVector3D(1,0,0) );
    m_shaderProgram->setUniformValue("ModelMatrix",  m_Transform.toMatrix() * localX);
    vecotorModel->draw();
    m_shaderProgram->setUniformValue("mMaterial.ambient",QVector3D(0,0,1) );
    m_shaderProgram->setUniformValue("mMaterial.diffuse", QVector3D(0,0,1) );
    m_shaderProgram->setUniformValue("ModelMatrix",  m_Transform.toMatrix() * localZ);
    vecotorModel->draw();
}

void Manipulator::drawPickingBuffer()
{
//    m_framebuffer->bind();
//    glEnable(GL_DEPTH_TEST);
//    QVector3D test = scene->m_arcCamera.worldPos();
//    qDebug()<<test;

    GLfloat a = 1.0;
     GLfloat b = 2.0;
      GLfloat c = 3.0;
    m_pickingProgram->setUniformValue("gDrawIndex",  5);
    m_pickingProgram->setUniformValue("test",  scene->m_arcCamera.worldPos());
    m_pickingProgram->setUniformValue("ModelMatrix",  m_Transform.toMatrix());
    m_pickingProgram->setUniformValue("gObjectIndex",  a);
    m_pickingProgram->setUniformValue("color",  QVector3D(0,1,0));
    vecotorModel->draw();
    m_pickingProgram->setUniformValue("ModelMatrix",  m_Transform.toMatrix() * localX);
    m_pickingProgram->setUniformValue("gObjectIndex",  b);
    m_pickingProgram->setUniformValue("color",  QVector3D(1,0,0));
    vecotorModel->draw();
    m_pickingProgram->setUniformValue("ModelMatrix",  m_Transform.toMatrix() * localZ);
    m_pickingProgram->setUniformValue("gObjectIndex",  c);
    m_pickingProgram->setUniformValue("color",  QVector3D(0,0,1));
    vecotorModel->draw();
}

void Manipulator::drawPickingBufferOld()
{
//    m_pickingProgram->bind();
    GLfloat a = 1.0;
     GLfloat b = 2.0;
      GLfloat c = 3.0;
    m_pickingProgram->setUniformValue("gDrawIndex",  5);
    m_pickingProgram->setUniformValue("test",  scene->m_arcCamera.worldPos());
    m_pickingProgram->setUniformValue("ModelMatrix",  m_Transform.toMatrix());
    m_pickingProgram->setUniformValue("gObjectIndex",  a);
    m_pickingProgram->setUniformValue("color",  QVector3D(0,1,0));
    vecotorModel->draw();
    m_pickingProgram->setUniformValue("ModelMatrix",  m_Transform.toMatrix() * localX);
    m_pickingProgram->setUniformValue("gObjectIndex",  b);
    m_pickingProgram->setUniformValue("color",  QVector3D(1,0,0));
    vecotorModel->draw();
    m_pickingProgram->setUniformValue("ModelMatrix",  m_Transform.toMatrix() * localZ);
    m_pickingProgram->setUniformValue("gObjectIndex",  c);
    m_pickingProgram->setUniformValue("color",  QVector3D(0,0,1));
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
        QPoint screenMouse = m_window->getMouseScreenCoords();
        int3 pixel = scene->readPixel(screenMouse.x(), screenMouse.y());

        switch(pixel.i)
        {
            case TRANSLATE_X:
                break;

            case TRANSLATE_Y:
                break;
        }



    }
}
