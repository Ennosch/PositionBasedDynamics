#ifndef ABSTRACTCONSTRAINT_H
#define ABSTRACTCONSTRAINT_H

#include <memory>
#include <vector>

#include  <QDebug>
#include <QVector3D>

#include "utils.h"

class Particle;

class AbstractConstraint
{
public:
    AbstractConstraint();
    inline virtual void project(){}
    inline virtual void projectPrevious(){}
    inline virtual float constraintFunction(){qDebug()<<" Abstract C"; return 1.0;}
    inline virtual QVector3D deltaP(){}

    inline virtual void debug(int _v){mlog<<" Abstract debug: "<<_v;}

// members:
    std::vector<ParticleWeakPtr> m_Particles;
    bool m_dirty = true;
    void setDirty(bool _isDirty);
};


#endif // ABSTRACTCONSTRAINT_H
