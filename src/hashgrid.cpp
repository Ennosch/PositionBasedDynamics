#include <boost/functional/hash.hpp>
#include <boost/math/distributions.hpp>
#include <math.h>

#include "hashgrid.h"

#include <QDebug>

HashGrid::HashGrid(float _cellSize)
{
    qDebug("HashGrid Particle tmpl ctor 2");
//    auto a = boost::math::iround<float>(14.494);

}

void HashGrid::clear()
{
    m_buckets.clear();
}

bool HashGrid::insert(size_t _hashV, ParticlePtr _p)
{

    bool CellExists = false;
    if(cellExists(_hashV))
    {
        CellExists = true;
    }
    m_buckets[_hashV].push_back(_p);
    return CellExists;

}

bool HashGrid::cellExists(size_t _hashV)
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

size_t HashGrid::hashFunction(int3 _cell)
{
        // seed prime number
        size_t hash = 85523717;
        boost::hash_combine(hash, _cell.i);
        boost::hash_combine(hash, _cell.j);
        boost::hash_combine(hash, _cell.k);
        return hash;
}

size_t HashGrid::hashFunciton(float _i, float _j, float _k)
{
    size_t hash = 85523717;
    boost::hash_combine(hash, _i);
    boost::hash_combine(hash, _j);
    boost::hash_combine(hash, _k);
    return hash;
}

int3 HashGrid::pointToCell(float _x, float _y, float _z)
{
    int3 cell;
    // rounding to full integer - coversion
    // optionally use: auto a = boost::math::iround<float>(_x);
    cell.i = int(_x / cellSize);
    cell.j = int(_y / cellSize);
    cell.k = int(_z / cellSize);
    return cell;
}

std::list<ParticlePtr> HashGrid::query(size_t _hash)
{
    std::list<ParticlePtr> cellList;
    if(!cellExists(_hash))
    {
        return cellList;
    }
    else
    {
        auto test = m_buckets[_hash];
        return m_buckets[_hash];
    }
}

std::list<ParticlePtr> HashGrid::cellNeighbours(int3 _cell)
{
    std::list<ParticlePtr> neighbourParticles;
    std::list<int3> neighbourCells;
    size_t middleHash = hashFunction(_cell);

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
                size_t hash = hashFunction(nCell);
//                if(hash == middleHash) continue;

                if(cellExists(hash))
                {
//                    qDebug()<<"Found a hash: "<<x<<y<<z;
                    std::list<ParticlePtr> currentCellList = query(hash);
                    std::list<ParticlePtr>::const_iterator itr = neighbourParticles.end();
                    std::list<ParticlePtr>::const_iterator itrNCellStart = currentCellList.begin();
                    std::list<ParticlePtr>::const_iterator itrNCellEnd = currentCellList.end();
                    neighbourParticles.insert(itr, itrNCellStart, itrNCellEnd);
                };
//                qDebug()<<"N: "<<n;
                n++;
            }
        }
    }

//    qDebug()<<&neighbourParticles;
    return neighbourParticles;
}



































