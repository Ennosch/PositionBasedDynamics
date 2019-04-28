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
    qDebug()<<"ctor 2 shape. indecies vertices ";
    setupMesh();
}

Shape::Shape(std::vector<Vertex> &_vertices,
             std::vector<unsigned int> &_indices, std::vector<QVector3D> &_points, std::map<int, std::list<int> > &_pointsToVertsMap)
        :  m_indices(_indices), m_vertices(_vertices), m_points(_points) ,m_pointsToVerts(_pointsToVertsMap)
{
    qDebug()<<"ctor 3 shape. indecies vertices ";
    setupMesh();
}

Shape::Shape(std::vector<Vertex> &_vertices,
             std::vector<unsigned int> &_indices,
             Scene *_scene,
             QOpenGLShaderProgram *_shaderProgram)
        :  m_indices(_indices), m_vertices(_vertices)
{
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
    glVertexAttribPointer(0,                // vert attr index
                          3,                // num componets
                          GL_FLOAT,         // type
                          GL_FALSE,         // normalized
                          sizeof(Vertex),   // stride
                          nullptr);         // pointer to first components. (offset)
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

void Shape::recomputeNormals()
{
    for(int i=0; i < m_indices.size(); i++)
    {
        if(!((i+1) % 3))
        {
            QVector3D vertA = m_vertices[m_indices[i-2]].Position;
            QVector3D vertB = m_vertices[m_indices[i-1]].Position;
            QVector3D vertC = m_vertices[m_indices[i  ]].Position;

            QVector3D AB, AC, CB, normal;
            AB  = vertB -vertA;
            AC  = vertC -vertA;
            normal = QVector3D::crossProduct(AB, AC).normalized();

            m_vertices[m_indices[i-2]].Normal = normal;
            m_vertices[m_indices[i-1]].Normal = normal;
            m_vertices[m_indices[i  ]].Normal = normal;
        }
    }
}

void Shape::updateVertexBuffer()
{
    recomputeNormals();

    m_pVao->bind();
    m_vvbo.create();
    m_vvbo.bind();
    m_vvbo.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    m_vvbo.allocate(m_vertices.data(), m_vertices.size() * sizeof(Vertex));

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

    m_pVao->release();
}

void Shape::draw()
{
    m_pVao->bind();
    unsigned int count = m_indices.size();
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
//       glDrawArrays(GL_TRIANGLES, 0, count);
    m_pVao->release();
}

void Shape::drawPoints()
{
    m_pVao->bind();
    unsigned int count = m_indices.size();
    glDrawElements(GL_POINTS, count, GL_UNSIGNED_INT, nullptr);
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

std::vector<Vertex>& Shape::getVertices()
{
    return m_vertices;
}

std::vector<QVector3D> &Shape::getPoints()
{
    return m_points;
}

std::map<int, std::list<int> > &Shape::getVertsMap()
{
    return m_pointsToVerts;
}

std::vector<unsigned int>& Shape::getIndices()
{
    return m_indices;
}

Vertex Shape::getVertexAtIndex(unsigned int idx)
{
    return m_vertices[idx];
}

void Shape::setVertexPositionAtIndex(unsigned int idx, const QVector3D _value)
{
    m_vertices[idx].Position = _value;
}



















