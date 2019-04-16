#ifndef RigidBody_H
#define RigidBody_H

#include "transform.h"
#include "dynamicObject.h"
#include "utils.h"
#include "dynamics/constraint.h"

class RigidBody : public DynamicObject
{
public:
    RigidBody();
    RigidBody(ModelPtr _model);

    void parseData(const ModelPtr _model);
    void addParticle(const QVector3D &_localPos, const ParticleWeakPtr _particle);
    ConstraintPtr createConstraint();

    void setTransform(const Transform _trans);

    // virtuals
    void setTransform(const QMatrix4x4& _mat4);
    void pinToPosition(const QVector3D &_pos);
    void endPinToPosition();

    const QMatrix4x4 &getTransfrom();
    const QVector3D getTranslation();

private:
    struct RestShape{
        QVector3D localPos;
        ParticleWeakPtr particle;
    };

    friend ShapeMatchingConstraint;
    int id;

    std::vector<RestShape> m_restShape;
    ModelPtr m_model;
    ShapePtr m_shape;

    Transform m_transfrom;
    std::vector<ParticleWeakPtr> m_particles;

};

#endif // RigidBody_H
