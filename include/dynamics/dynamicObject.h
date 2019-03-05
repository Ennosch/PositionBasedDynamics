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
    DynamicObject( ParticlePtr _particle);
    void setTransform(const QMatrix4x4& _mat4);
    void pinToPosition(const QVector3D &_pos);
    void endPinToPosition();
    ParticlePtr particle();

    const QMatrix4x4& getTransfrom();
    const QVector3D getTranslation();
private:
    bool m_IsParticle = false;
    Transform m_transform;
    ParticlePtr pParticle;


//    std::vector<QVector3D> m_particles;
//    std::vector<unsigned int> m_indices;

};

#endif // DYNAMICOBJECT_H
