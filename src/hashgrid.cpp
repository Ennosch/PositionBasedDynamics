#include <boost/functional/hash.hpp>
#include <boost/math/distributions.hpp>.
#include <math.h>

#include "hashgrid.h"

#include <QDebug>

template <typename T>

HashGrid<T>::HashGrid(float _cellSize)
{

}


template<> HashGrid<ParticlePtr>::HashGrid(float _cellSize)
{
    qDebug("HashGrid Particle tmpl ctor 2");
//    auto a = boost::math::iround<float>(14.494);

}

template<typename T>
bool HashGrid<T>::insert(size_t _hashV, ParticlePtr _p)
{

}

template<> bool HashGrid<ParticlePtr>::insert(size_t _hashV, ParticlePtr _p)
{
    bool CellExists = false;
    std::unordered_map< size_t , std::list< ParticlePtr >>::const_iterator itr = m_buckets.find(_hashV);
    if ( itr == m_buckets.end())
    {
        CellExists = false;
    }
    else
    {
        CellExists = true;
    }
    m_buckets[_hashV].push_back(_p);
//    if(cellExists(_hashV))
//    {
//        CellExists = true;
//    }
//    else
//    {
//        CellExists = false;
//    }
    return CellExists;
}

template<typename T>
bool HashGrid<T>::cellExists(size_t _hashV)
{
    bool CellExists = false;
    std::unordered_map< size_t , std::list< ParticlePtr >>::const_iterator itr = m_buckets.find(_hashV);
    if ( itr == m_buckets.end())
    {
        CellExists = false;
    }
    else
    {
        CellExists = true;
    }
    return CellExists;
}

template<> bool HashGrid<ParticlePtr>::cellExists(size_t _hashV)
{
    bool CellExists = false;
    std::unordered_map< size_t , std::list< ParticlePtr >>::const_iterator itr = m_buckets.find(_hashV);
    if ( itr == m_buckets.end())
    {
        CellExists = false;
    }
    else
    {
        CellExists = true;
    }
    return CellExists;
}




template<typename T>
size_t HashGrid<T>::hashFunction(int3 _cell)
{
        qDebug("hashFunction template T");

}


template<> size_t HashGrid<ParticlePtr>::hashFunction(int3 _cell)
{
        qDebug("hashFunction ParticlePtr");
        // seed prime number
        size_t hash = 85523717;
        boost::hash_combine(hash, _cell.i);
        boost::hash_combine(hash, _cell.j);
        boost::hash_combine(hash, _cell.k);
        return hash;
}

template<typename T>
int3 HashGrid<T>::pointToCell(float _x, float _y, float _z)
{

}

template<> int3 HashGrid<ParticlePtr>::pointToCell(float _x, float _y, float _z)
{
    int3 cell;
    // rounding to full integer - coversion
    // optionally use: auto a = boost::math::iround<float>(_x);
    cell.i = int(_x / cellSize);
    cell.i = int(_y / cellSize);
    cell.i = int(_z / cellSize);
    return cell;
}

template<typename T>
std::list<ParticlePtr> HashGrid<T>::cellNeighbours(int3 _cell)
{

}


template<> std::list<ParticlePtr> HashGrid<ParticlePtr>::cellNeighbours(int3 _cell)
{
    std::list<ParticlePtr> neighbourParticles;
    std::list<int3> neighbourCells;

    int level = 1;
    int length = level + 2;
    int size = std::pow(length, 3);
    int n = 0;

    for(int y = -1 ; y <= level ; y++)
    {
        for(int x = -1 ; x <= level ; x++)
        {
            for(int z = -1 ; z <= level ; z++)
            {
                int3 nCell;
                nCell.i = _cell.i + x;
                nCell.j = _cell.j + y;
                nCell.k = _cell.k + z;

                neighbourCells.push_back(nCell);
                // query for std::list<ParticlePtr>
                // and if possible merge
//                qDebug()<<"N: "<<n;
                n++;
            }
        }
    }


    return neighbourParticles;
}



































