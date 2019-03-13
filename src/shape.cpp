#include "shape.h"
#include <iostream>
#include <QDebug>



Shape::Shape()
{
    qDebug()<<"ctor 1.1 Shape";
}

Shape::Shape(std::vector<Vertex> &_vertices,
             std::vector<unsigned int> &_indices)
        :  m_indices(_indices), m_vertices(_vertices)
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
        :  m_indices(_indices), m_vertices(_vertices)
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
                      m_vertices(_rhs.m_vertices),
                      m_indices(_rhs.m_indices)
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
//    m_vertices = _data;
//    m_verticesSize = _size;
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
    m_vvbo.allocate(m_vertices.data(), m_vertices.size() * sizeof(Vertex));

    m_ebo = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    m_ebo.create();
    m_ebo.bind();
    m_ebo.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    m_ebo.allocate(m_indices.data(), m_indices.size()*4);

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
       unsigned int count = m_indices.size();
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
    unsigned int count= m_indices.size();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    m_pVao->release();
}

std::vector<Vertex> Shape::getVertices()
{
    return m_vertices;
}

std::vector<unsigned int> Shape::getIndices()
{
    return m_indices;
}

Vertex Shape::getVertexAtIndex(unsigned int idx)
{
    return m_vertices[idx];
}



















