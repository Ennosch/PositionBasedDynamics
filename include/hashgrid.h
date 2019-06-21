#ifndef HASHGRID_H
#define HASHGRID_H

#include <unordered_map>
#include <map>
#include <list>

#include "utils.h"
#include "dynamics/particle.h"


class HashGrid
{
public:
//    HashGrid();
    HashGrid(float _cellSize = 1.0);

    void clear();
    void setGridSize(float _size);
    float getGridSize();

    bool insert(size_t _hashV, ParticlePtr _p);
    bool cellExists(size_t _hashV);
    size_t hashFunction(int3 _cell);
    size_t hashFunciton(float _i, float _j, float _k);
    int3 pointToCell(float _x, float _y, float _z);
    std::list<ParticlePtr> query(size_t _hash);
    std::list<ParticlePtr> cellNeighbours(int3 _cell);

//  query

private:
    float cellSize = 1.0;
    std::unordered_map< size_t , std::list< ParticlePtr >> m_buckets;

};

#endif // HASHGRID_H
