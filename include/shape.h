#ifndef SHAPE_H
#define SHAPE_H
#include <QDebug>

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QVector3D>
#include <vector>



struct Vertex {
    // position
    QVector3D Position;
    // normal
    QVector3D Normal;
};

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
    Shape(int _id): m_Id(_id){ qDebug()<<"Ctor 2 Shape";};
    Shape(std::vector<Vertex> &_vertices, std::vector<unsigned int> &_indices);

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
    Shape(const Shape& _rhs) :m_Id(_rhs.m_Id){};
    // creates buffer, add a name
    void allocate(const QVector3D *_data, int _size);
    void release();
    void bind();
    void setupMesh();
    void draw();

    inline void foo(){qDebug("foo");};
    inline int getVertsSize(){return m_verticesSize;};
    inline const QVector3D* getData(){return m_vertices; };

//-----members-------
private:
    int m_Id;
    ShapeType type;
    QOpenGLBuffer m_vvbo;
    QOpenGLBuffer m_ebo;
    QOpenGLVertexArrayObject m_vao;
    std::string m_name, directory;
    const QVector3D *m_vertices;
    int m_verticesSize;

    // WIP model loading
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

};

inline void Shape::bind(){m_vao.bind(); };


#endif // SHAPE_H
