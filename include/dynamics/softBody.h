#ifndef SOFTBODY_H
#define SOFTBODY_H

//#include <transform.h>
#include "utils.h"
#include "dynamics/dynamicUtils.h"
#include "model.h"

class softBody
{
public:
    softBody();
    softBody(ModelPtr _model);

private:
    ModelPtr m_model;
    std::vector<ParticleWeakPtr> m_particles;
};

#endif // SOFTBODY_H
