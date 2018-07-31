#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <memory>
#include <unordered_map>

#include <QDebug>

class Shape;
class Model;
class SceneObject;


// (?) utils should become a class to be able to forward declare and have a nameSpace ?

inline void utilsFoo()
{
    qDebug()<<" hello Utils 2 !";
}


typedef std::unordered_map <std::string, std::shared_ptr <Shape>>       ShapeMap;
typedef std::unordered_map <std::string, std::shared_ptr <Model>>       ModelMap;

typedef std::shared_ptr <SceneObject>       pSceneOb;
typedef std::shared_ptr <Shape>             ShapePtr;
typedef std::shared_ptr <Model>             ModelPtr;

#endif // UTILS_H
