#include "shape.h"
#include <iostream>
#include <QDebug>

Shape::Shape()
{
    qDebug()<<"ctor 1 Shape";
    //m_vao.create();
}

Shape::Shape(std::vector<Vertex> &_vertices, std::vector<unsigned int> &_indices)
        :  indices(_indices)
{
//        for(std::vector<unsigned int>::reverse_iterator it = indices.rbegin(); it != indices.rend(); ++it)
//        {
//            std::cout << *it <<std::endl;
//        }
//    vertices = _vertices;


    for(int i = 0; i < indices.size(); i+=3)
    {

        auto f1 = indices[i]+1;
        auto f2 = indices[i]+2;
        auto f3 = indices[i]+3;

//        auto v11 = vertices[i].Position;
//        auto v12 = vertices[i+1].Position;
//        auto v13 = vertices[i+2].Position;

//        qDebug()<<"f"<<f1<<f2<<f3<<"v:"<<v11<<"|"<<v12<<"|"<<v13;
    }
//    qDebug()<<indices.size() / 3;

//    qDebug()<<_vertices[0];
//    setupMesh();
}


Shape& Shape::operator=(const Shape &_rhs)
{
    qDebug()<<"copy assignemnt Shape";
//    *this->m_vao.create();
    return *this;
}

void Shape::allocate(const QVector3D* _data, int _size)
{
    m_vao.create();
    m_vao.bind();
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

    m_vao.create();
    m_vao.bind();

    m_vvbo.create();
    m_vvbo.bind();
    m_vvbo.allocate(vertices.data(), sizeof(vertices));

    m_ebo = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    m_ebo.create();
    m_ebo.bind();
    m_ebo.allocate(indices.data(), sizeof(indices));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));


    m_ebo.release();
    m_vvbo.release();
    m_vao.release();
}

void Shape::draw()
{
//    qDebug()<<"enter Draw";
    m_vao.bind();
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    m_vao.release();
}
