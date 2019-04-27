#ifndef DYNAMICUTILS_H
#define DYNAMICUTILS_H

#include <vector>
#include <memory>
#include <algorithm>
#include <list>
#include <map>
#include <set>

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

template<typename K, typename V>
K findKbyValueElement(std::map<K, V> map, K elementOfV)
{
    typename std::map<K,V>::iterator it;
    it = std::find_if(map.begin(), map.end(), [elementOfV](std::pair<K,V> pair){
            return std::any_of(pair.second.begin(), pair.second.end(), [elementOfV](K _x){
                return(_x == elementOfV);
                });
         });
    // currently returning a 0 is a valid result
    // hence a -1 need to show no found result. But that way function wont work with other types (?)
    if(it == map.end())
        return -1;
    return it->first;
};

template<typename T>
bool hasPair(std::vector<std::set<T>> pairsVec, std::set<T> pair)
{
    return std::any_of(pairsVec.begin(), pairsVec.end(), [pair](std::set<T> _x)
    {
        return (pair == _x);
    });
};


#endif // DYNAMICUTILS_H


