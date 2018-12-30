#ifndef ABSTRACTCONSTRAINT_H
#define ABSTRACTCONSTRAINT_H

#include  <QDebug>
#include <QVector3D>

class AbstractConstraint
{
public:
    AbstractConstraint();
    inline virtual void project(){}
    inline virtual float constraintFunction(){qDebug()<<" Abstract C"; return 1.0;}
    inline virtual QVector3D deltaP(){}
};


#endif // ABSTRACTCONSTRAINT_H
