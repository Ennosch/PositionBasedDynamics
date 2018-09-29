#ifndef DYNAMICOBJECT_H
#define DYNAMICOBJECT_H

#include <vector>

#include <QVector3D>

#include <transform.h>
#include <model.h>



class DynamicObject
{
public:
    DynamicObject();
private:
    Transform m_transform;

//    std::vector<QVector3D> m_particles;
//    std::vector<unsigned int> m_indices;

};

#endif // DYNAMICOBJECT_H
