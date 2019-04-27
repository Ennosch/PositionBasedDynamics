#ifndef RigidBody_H
#define RigidBody_H

#include "transform.h"
#include "dynamicObject.h"
#include "dynamics/dynamicUtils.h"
#include "dynamics/constraint.h"

class RigidBody : public DynamicObject,  public std::enable_shared_from_this<RigidBody>
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
    void updateModelBuffers();

    ModelPtr getModel();
    const QMatrix4x4 &getTransfrom();
    const QVector3D getTranslation();


private:
    friend ShapeMatchingConstraint;
    int id;

    std::vector<QVector3D> m_restShape;
    ModelPtr m_model;
    ShapePtr m_shape;

    Transform m_transfrom;
    std::vector<ParticleWeakPtr> m_particles;

};

#endif // RigidBody_H
