
#include "shape.h"

#include <QDebug>

Shape::Shape()
{
    qDebug()<<"ctor 1 Shape";
    //m_vao.create();
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




