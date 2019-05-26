#ifndef DYNAMICOBJECT_H
#define DYNAMICOBJECT_H

#include <vector>

#include <QVector3D>

#include <transform.h>
#include <model.h>

#include "dynamics/dynamicUtils.h"



class DynamicObject
{
public:
    DynamicObject();

    virtual void pinToPosition(const QVector3D &_pos);
    virtual void endPinToPosition();

//    virtual DynamicObject* pointer();
    virtual ParticlePtr pointer(Particle *ptr){
        return nullptr;
    }
//    virtual void pointer(Particle** _ptr);

    virtual const QMatrix4x4 getTransfrom();
    virtual const QVector3D getTranslation();
    virtual std::vector<ParticleWeakPtr>& getParticles(){ std::vector<ParticleWeakPtr> vec; return vec; }

    std::vector<ConstraintWeakPtr> m_Constraints;

    int mID;
private:
//    bool m_IsParticle = false;

//    Transform m_transform;
//    ParticlePtr pParticle;

//    std::vector<QVector3D> m_particles;
//    std::vector<unsigned int> m_indices;
};

#endif // DYNAMICOBJECT_H
