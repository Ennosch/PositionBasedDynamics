#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <memory>
#include <unordered_map>
#include <math.h>
#include <stdlib.h>
#include <iostream>

#include <QOpenGLFunctions>
#include <QVector3D>

#include <QDebug>


#define myqDebug() qDebug() << fixed << qSetRealNumberPrecision(2)


// Front Verticies
#define VERTEX_FTR QVector3D( 0.5f,  0.5f,  0.5f), QVector3D( 1.0f, 0.0f, 0.0f )
#define VERTEX_FTL QVector3D(-0.5f,  0.5f,  0.5f), QVector3D( 0.0f, 1.0f, 0.0f )
#define VERTEX_FBL QVector3D(-0.5f, -0.5f,  0.5f), QVector3D( 0.0f, 0.0f, 1.0f )
#define VERTEX_FBR QVector3D( 0.5f, -0.5f,  0.5f), QVector3D( 0.0f, 0.0f, 0.0f )

// Back Verticies
#define VERTEX_BTR QVector3D( 0.5f,  0.5f, -0.5f), QVector3D( 1.0f, 1.0f, 0.0f )
#define VERTEX_BTL QVector3D(-0.5f,  0.5f, -0.5f), QVector3D( 0.0f, 1.0f, 1.0f )
#define VERTEX_BBL QVector3D(-0.5f, -0.5f, -0.5f), QVector3D( 1.0f, 0.0f, 1.0f )
#define VERTEX_BBR QVector3D( 0.5f, -0.5f, -0.5f), QVector3D( 1.0f, 1.0f, 1.0f )

class Shape;
class Model;
class SceneObject;

struct Material {
    QVector3D ambient;
    QVector3D diffuse;
    QVector3D specular;
    float shininess;
};

struct Light
{
    QVector3D position;
    QVector3D   color;
};

struct Vertex {
    // position
    QVector3D Position;
    // normal
    QVector3D Normal;
};

typedef std::unordered_map <std::string, std::shared_ptr <Shape>>       ShapeMap;
typedef std::unordered_map <std::string, std::shared_ptr <Model>>       ModelMap;

typedef std::shared_ptr <SceneObject>       pSceneOb;
typedef std::shared_ptr <Shape>             ShapePtr;
typedef std::shared_ptr <Model>             ModelPtr;
typedef std::shared_ptr <Material>          MaterialPtr;
typedef std::shared_ptr <Light>             LightPtr;


// (?) utils should become a class to be able to forward declare and have a nameSpace ?

inline void utilsFoo()
{
    qDebug()<<" hello Utils 2 !";
}

inline float randf(int _mod)
{
    float r10 = std::rand() % _mod;
    float r1 = (std::rand() % _mod) / 10.0;
    if(r10 == 0.0) r10 = 1;
    if(r10 == 0.0) r1 = 0.1;

//    qDebug()<<"randf is: "<< r10<<"rand1 is: "<< r1 <<" result: "<<r10 * r1;
    return r10 * r1;
}

inline float randfinRange(int a, int b)
{
    float r1 = a + (std::rand() % ( b - a + 1));
    float r2 = (a + (std::rand() % ( b - a + 1))) * 0.1;
//    qDebug()<<r1<<" : "<<r2;
    return r1 + r2;
}

inline QVector3D randVec3(int _mod)
{
    QVector3D randomVec = QVector3D(randf(_mod), randf(_mod), randf(_mod));
//    qDebug()<<"randVec3 is: "<<randomVec;
    return randomVec;
}




static const GLfloat X = 0.525731112119133606;
static const GLfloat Z = 0.850650808352039932;

//---------------------------  define some Geometry ---------------------------



static const float quad[] = {
  -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
   1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
   1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
   1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
  -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
  -1.0f, -1.0f, 0.0f, 0.0f, 0.0f
};

static const  GLfloat vtxIcosahedron[12][3] = {
    {-X, 0.0f, Z}, {X, 0.0f, Z}, {-X, 0.0f, -Z}, {X, 0.0f, -Z},
    {0.0f, Z, X}, {0.0f, Z, -X}, {0.0f, -Z, X}, {0.0f, -Z, -X},
    {Z, X, 0.0}, {-Z, X, 0.0f}, {Z, -X, 0.0f}, {-Z, -X, 0.0f}
 };

static const GLuint indxIcosahedron[20][3] = {
    {0,  4,  1}, {0, 9,  4}, {9,  5, 4}, { 4, 5, 8}, {4, 8,  1},
    {8, 10,  1}, {8, 3, 10}, {5,  3, 8}, { 5, 2, 3}, {2, 7,  3},
    {7, 10,  3}, {7, 6, 10}, {7, 11, 6}, {11, 0, 6}, {0, 1,  6},
    {6,  1, 10}, {9, 0, 11}, {9, 11, 2}, { 9, 2, 5}, {7, 2, 11}};

static const QVector3D CubeWithNormals[] = {
    // Face 1 (Front)
      VERTEX_FTR, QVector3D(0.0f, 0.0f, 1.0f), VERTEX_FTL, QVector3D(0.0f, 0.0f, 1.0f), VERTEX_FBL, QVector3D(0.0f, 0.0f, 1.0f),
      VERTEX_FBL, QVector3D(0.0f, 0.0f, 1.0f), VERTEX_FBR, QVector3D(0.0f, 0.0f, 1.0f), VERTEX_FTR, QVector3D(0.0f, 0.0f, 1.0f),
    // Face 2 (Back)
      VERTEX_BBR, QVector3D(0.0f, 0.0f, -1.0f), VERTEX_BTL, QVector3D(0.0f, 0.0f, -1.0f), VERTEX_BTR, QVector3D(0.0f, 0.0f, -1.0f),
      VERTEX_BTL, QVector3D(0.0f, 0.0f, -1.0f), VERTEX_BBR, QVector3D(0.0f, 0.0f, -1.0f), VERTEX_BBL, QVector3D(0.0f, 0.0f, -1.0f),
    // Face 3 (Top)
      VERTEX_FTR, QVector3D(0.0f, 1.0f, 0.0f), VERTEX_BTR, QVector3D(0.0f, 1.0f, 0.0f), VERTEX_BTL, QVector3D(0.0f, 1.0f, 0.0f),
      VERTEX_BTL, QVector3D(0.0f, 1.0f, 0.0f), VERTEX_FTL, QVector3D(0.0f, 1.0f, 0.0f), VERTEX_FTR, QVector3D(0.0f, 1.0f, 0.0f),
    // Face 4 (Bottom)
      VERTEX_FBR, QVector3D(0.0f, -1.0f, 0.0f), VERTEX_FBL, QVector3D(0.0f, -1.0f, 0.0f), VERTEX_BBL, QVector3D(0.0f, -1.0f, 0.0f),
      VERTEX_BBL, QVector3D(0.0f, -1.0f, 0.0f), VERTEX_BBR, QVector3D(0.0f, -1.0f, 0.0f), VERTEX_FBR, QVector3D(0.0f, -1.0f, 0.0f),
    // Face 5 (Left)
      VERTEX_FBL, QVector3D(-1.0f, 0.0f, 0.0f), VERTEX_FTL, QVector3D(-1.0f, 0.0f, 0.0f), VERTEX_BTL, QVector3D(-1.0f, 0.0f, 0.0f),
      VERTEX_FBL, QVector3D(-1.0f, 0.0f, 0.0f), VERTEX_BTL, QVector3D(-1.0f, 0.0f, 0.0f), VERTEX_BBL, QVector3D(-1.0f, 0.0f, 0.0f),
    // Face 6 (Right)
      VERTEX_FTR, QVector3D(1.0f, 0.0f, 0.0f), VERTEX_FBR, QVector3D(1.0f, 0.0f, 0.0f), VERTEX_BBR, QVector3D(1.0f, 0.0f, 0.0f),
      VERTEX_BBR, QVector3D(1.0f, 0.0f, 0.0f), VERTEX_BTR, QVector3D(1.0f, 0.0f, 0.0f), VERTEX_FTR, QVector3D(1.0f, 0.0f, 0.0f)
};

// define in Vertex format wip
static const Vertex CubeWithNormalsDash[] = {
//    Vertex()
};

#endif // UTILS_H
