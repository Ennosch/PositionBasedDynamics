#include "shape.h"
#include <iostream>
#include <QDebug>



Shape::Shape()
{
    qDebug()<<"ctor 1.1 Shape";
}

Shape::Shape(std::vector<Vertex> &_vertices,
             std::vector<unsigned int> &_indices)
        :  indices(_indices), vertices(_vertices)
{
    // No parent, no automatic deletion ?!
    // The parent of an object may be viewed as the object's owner
//    m_pVao = new QOpenGLVertexArrayObject();


    qDebug()<<"ctor 2 shape. indecies vertices ";
    setupMesh();
    int v = 4;
}

Shape::Shape(std::vector<Vertex> &_vertices,
             std::vector<unsigned int> &_indices,
             Scene *_scene,
             QOpenGLShaderProgram *_shaderProgram)
        :  indices(_indices), vertices(_vertices)
{
    // No parent, no automatic deletion ?!
    // The parent of an object may be viewed as the object's owner
    qDebug()<<"construct Shape with Scene and Shader";

    setupMesh();
}


Shape& Shape::operator=(const Shape &_rhs)
{
    qDebug()<<"copy assignemnt Shape";
    return *this;
}

Shape::Shape(const Shape &_rhs)
                    :m_Id(_rhs.m_Id),
                      vertices(_rhs.vertices),
                      indices(_rhs.indices)
{
    qDebug()<<"copy constructor Shape";
    m_pVao = _rhs.m_pVao;
    m_ebo = _rhs.m_ebo;
    m_vvbo = _rhs.m_vvbo;
}

void Shape::allocate(const QVector3D* _data, int _size)
{
//    m_vao.create();
    m_pVao = new QOpenGLVertexArrayObject();
    m_pVao->create();
    m_pVao->bind();
    m_vvbo.create();
    m_vvbo.bind();
    m_vvbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vvbo.allocate(_data, _size);
    m_vertices = _data;
    m_verticesSize = _size;
}

void Shape::release()
{
    m_vao.release();
    m_vvbo.release();
}

void Shape::setupMesh()
{
    m_pVao = new QOpenGLVertexArrayObject();
    m_pVao->create();
    m_pVao->bind();

    m_vvbo.create();
    m_vvbo.bind();
    m_vvbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    // vertices.size() * 24 perviously here
    m_vvbo.allocate(vertices.data(), vertices.size() * sizeof(Vertex));

    m_ebo = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    m_ebo.create();
    m_ebo.bind();
    m_ebo.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    m_ebo.allocate(indices.data(), indices.size()*4);

    // theory this call is not setting this in the shader/ or doesn't reacht the GL context
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (void*)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          (void*)offsetof(Vertex, Barycentric));
}

void Shape::draw()
{
       m_pVao->bind();
       unsigned int count = indices.size();
       glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
//       glDrawArrays(GL_TRIANGLES, 0, count);
        m_pVao->release();
}

void Shape::drawWireframe()
{

}

void Shape::drawOld()
{
    m_pVao->bind();
    unsigned int count= indices.size();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    m_pVao->release();
}



















