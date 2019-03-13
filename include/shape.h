#ifndef SHAPE_H
#define SHAPE_H
#include <QDebug>

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QVector3D>
#include <vector>

#include <QtGui/QOpenGLShaderProgram>

#include "utils.h"



class Scene;

class Shape
{
public:
    enum ShapeType
    {
        CUBE,
        SHPERE,
        OBJ
    };
    // ctor
    Shape();
    Shape(int _id): m_Id(_id){ qDebug()<<"Ctor 2 Shape";}
    Shape(std::vector<Vertex> &_vertices, std::vector<unsigned int> &_indices);
    Shape(std::vector<Vertex> &_vertices,
          std::vector<unsigned int> &_indices,
          Scene *_scene,
          QOpenGLShaderProgram *_shaderProgram);

    /*
     * user def copy assigment, copy assigment for QOpenGLVertexArrayObject has been deleted
    // C++11 rule, if T has member cannot be copy-assigned, = is implicitly delted
    // (?) Why does Qt explicitly delete operator = ?
    /
     * error: object of type 'Shape' cannot be assigned because its copy assignment operator is implicitly deleted
    a = Shape();
    note: copy assignment operator of 'Shape' is implicitly deleted because field 'm_vao' has a deleted copy assignment operator
     */
    Shape& operator=(const Shape& _rhs);
    /*similiar problem as above
     implicit copy constructor has been deleted for QOpenGLVertexArrayObject
     */
    Shape(const Shape& _rhs);
    // creates buffer, add a name
    void allocate(const QVector3D *_data, int _size);
    void release();
    void bind();
    void setupMesh();
    void draw();
    void drawWireframe();
    void drawOld();
    Vertex* data();
    unsigned int getNumVertices();

    std::vector<Vertex>  getVertices();
    std::vector<unsigned int> getIndices();
    Vertex getVertexAtIndex(unsigned int idx);

//    inline int getVertsSize(){return m_verticesSize;}
//    inline const QVector3D* getData(){return m_vertices; }

//-----members-------
private:
    int m_Id = 5;
    ShapeType type;
    QOpenGLBuffer m_vvbo;
    QOpenGLBuffer m_ebo;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLVertexArrayObject* m_pVao;
    std::string m_name, directory;
//    const QVector3D *m_vertices;
    int m_verticesSize;

    // WIP model loading
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;

    Scene *pScene;
    QOpenGLShaderProgram *pShader;

};

inline void Shape::bind(){m_pVao->bind(); };
inline Vertex* Shape::data(){ return m_vertices.data(); };
inline unsigned int Shape::getNumVertices(){ return m_indices.size(); };


#endif // SHAPE_H
