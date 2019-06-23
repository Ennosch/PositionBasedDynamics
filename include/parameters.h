#ifndef PARAMETERS_H
#define PARAMETERS_H

#include "QVector3D"

typedef QVector3D Vec3;

static Vec3 gravityDir(0,-9.8,0);
static float gravityLength                         = 9.8;
static float distanceConstraintStrechR             = 1.0;
static float distanceConstraintCompressR           = 1.0;
static float shapeMatchingGoalAttract              = 1.0;
static float frictionConstraintDynamicF            = 0.5;
static float frictionConstraintStaticF             = 0.5;
static float pbd_Damping                            = 0.03;

static int preConditionIterations                  = 2;
static int constraintIterations                    = 10;
static float timeStepSize                          = 0.02;
static float particleMass                          = .8;

static bool showParticles;


#endif // PARAMETERS_H
