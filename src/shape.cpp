
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



