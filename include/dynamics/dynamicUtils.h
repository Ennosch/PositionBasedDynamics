#ifndef DYNAMICUTILS_H
#define DYNAMICUTILS_H

#include <vector>
#include <memory>

class DynamicObject;
class Particle;
class AbstractConstraint;
class DistanceEqualityConstraint;
class RigidBody;

typedef std::shared_ptr <DynamicObject>         DynamicObjectPtr;
typedef std::shared_ptr <Particle>              ParticlePtr;
typedef std::weak_ptr <Particle>                ParticleWeakPtr;
typedef std::shared_ptr <AbstractConstraint>    ConstraintPtr;
typedef std::weak_ptr <AbstractConstraint>      ConstraintWeakPtr;

#endif // DYNAMICUTILS_H


