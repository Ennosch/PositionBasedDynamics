#include "include/dynamics/abstractconstraint.h"

#include "dynamics/particle.h"

AbstractConstraint::AbstractConstraint()
{
    //    qDebug()<<"INit abstract Constraint";
}

void AbstractConstraint::setDirty(bool _isDirty)
{
    m_dirty = _isDirty;
}
