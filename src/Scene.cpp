#include <array>
#include <iostream>
#include <math.h>
#include <stdlib.h>

#include <Qdebug>
#include <QFile>
#include <QDebug>
#include <QString>

#include "Scene.h"

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

#define myqDebug() qDebug() << fixed << qSetRealNumberPrecision(2)

static const float quad[] = {
  -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
   1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
   1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
   1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
  -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
  -1.0f, -1.0f, 0.0f, 0.0f, 0.0f
};

static const QVector3D myShape[] = {
    // Face 1 (Front)
      VERTEX_FTR, VERTEX_FTL, VERTEX_FBL,
      VERTEX_FBL, VERTEX_FBR, VERTEX_FTR,
    // Face 2 (Back)
      VERTEX_BBR, VERTEX_BTL, VERTEX_BTR,
      VERTEX_BTL, VERTEX_BBR, VERTEX_BBL,
    // Face 3 (Top)
      VERTEX_FTR, VERTEX_BTR, VERTEX_BTL,
      VERTEX_BTL, VERTEX_FTL, VERTEX_FTR,
    // Face 4 (Bottom)
      VERTEX_FBR, VERTEX_FBL, VERTEX_BBL,
      VERTEX_BBL, VERTEX_BBR, VERTEX_FBR,
    // Face 5 (Left)
      VERTEX_FBL, VERTEX_FTL, VERTEX_BTL,
      VERTEX_FBL, VERTEX_BTL, VERTEX_BBL,
    // Face 6 (Right)
      VERTEX_FTR, VERTEX_FBR, VERTEX_BBR,
      VERTEX_BBR, VERTEX_BTR, VERTEX_FTR
};

static const GLfloat X = 0.525731112119133606;
static const GLfloat Z = 0.850650808352039932;

static const  GLfloat vdata[12][3] = {
    {-X, 0.0f, Z}, {X, 0.0f, Z}, {-X, 0.0f, -Z}, {X, 0.0f, -Z},
    {0.0f, Z, X}, {0.0f, Z, -X}, {0.0f, -Z, X}, {0.0f, -Z, -X},
    {Z, X, 0.0}, {-Z, X, 0.0f}, {Z, -X, 0.0f}, {-Z, -X, 0.0f}
 };

static const GLuint tindices[20][3] = {
    {0,  4,  1}, {0, 9,  4}, {9,  5, 4}, { 4, 5, 8}, {4, 8,  1},
    {8, 10,  1}, {8, 3, 10}, {5,  3, 8}, { 5, 2, 3}, {2, 7,  3},
    {7, 10,  3}, {7, 6, 10}, {7, 11, 6}, {11, 0, 6}, {0, 1,  6},
    {6,  1, 10}, {9, 0, 11}, {9, 11, 2}, { 9, 2, 5}, {7, 2, 11}};

//static const QVector3D myShapeNormals[] = {
//    // Face 1 (Front)
//      VERTEX_FTR, QVector3D(0.0f, 0.0f, -1.0f), VERTEX_FTL, QVector3D(0.0f, 0.0f, -1.0f), VERTEX_FBL, QVector3D(0.0f, 0.0f, -1.0f),
//      VERTEX_FBL, QVector3D(0.0f, 0.0f, -1.0f), VERTEX_FBR, QVector3D(0.0f, 0.0f, -1.0f), VERTEX_FTR, QVector3D(0.0f, 0.0f, -1.0f),
//    // Face 2 (Back)
//      VERTEX_BBR, QVector3D(0.0f, 0.0f, 1.0f), VERTEX_BTL, QVector3D(0.0f, 0.0f, 1.0f), VERTEX_BTR, QVector3D(0.0f, 0.0f, 1.0f),
//      VERTEX_BTL, QVector3D(0.0f, 0.0f, 1.0f), VERTEX_BBR, QVector3D(0.0f, 0.0f, 1.0f), VERTEX_BBL, QVector3D(0.0f, 0.0f, 1.0f),
//    // Face 3 (Top)
//      VERTEX_FTR, QVector3D(0.0f, 1.0f, 0.0f), VERTEX_BTR, QVector3D(0.0f, 1.0f, 0.0f), VERTEX_BTL, QVector3D(0.0f, 1.0f, 0.0f),
//      VERTEX_BTL, QVector3D(0.0f, 1.0f, 0.0f), VERTEX_FTL, QVector3D(0.0f, 1.0f, 0.0f), VERTEX_FTR, QVector3D(0.0f, 1.0f, 0.0f),
//    // Face 4 (Bottom)
//      VERTEX_FBR, QVector3D(0.0f, -1.0f, 0.0f), VERTEX_FBL, QVector3D(0.0f, -1.0f, 0.0f), VERTEX_BBL, QVector3D(0.0f, -1.0f, 0.0f),
//      VERTEX_BBL, QVector3D(0.0f, -1.0f, 0.0f), VERTEX_BBR, QVector3D(0.0f, -1.0f, 0.0f), VERTEX_FBR, QVector3D(0.0f, -1.0f, 0.0f),
//    // Face 5 (Left)
//      VERTEX_FBL, QVector3D(-1.0f, 0.0f, 0.0f), VERTEX_FTL, QVector3D(-1.0f, 0.0f, 0.0f), VERTEX_BTL, QVector3D(-1.0f, 0.0f, 0.0f),
//      VERTEX_FBL, QVector3D(-1.0f, 0.0f, 0.0f), VERTEX_BTL, QVector3D(-1.0f, 0.0f, 0.0f), VERTEX_BBL, QVector3D(-1.0f, 0.0f, 0.0f),
//    // Face 6 (Right)
//      VERTEX_FTR, QVector3D(1.0f, 0.0f, 0.0f), VERTEX_FBR, QVector3D(1.0f, 0.0f, 0.0f), VERTEX_BBR, QVector3D(1.0f, 0.0f, 0.0f),
//      VERTEX_BBR, QVector3D(1.0f, 0.0f, 0.0f), VERTEX_BTR, QVector3D(1.0f, 0.0f, 0.0f), VERTEX_FTR, QVector3D(1.0f, 0.0f, 0.0f)
//};

static const QVector3D myShapeNormals[] = {
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


Scene::Scene(Window *_window) : AbstractScene(_window)
{

}

Scene::~Scene()
{

}
void Scene::initialize()
{
  AbstractScene::initialize();  
  QtOpenGLinitialize();

//  Scene::addModel(this, "model", "resources/objects/rock/Cube.obj");
}

void Scene::resize(int width, int height)
{
    m_projection_matrix.setToIdentity();
    m_projection_matrix.perspective(60.0f, width / float(height), 0.1f, 1000.0f);
}

void Scene::addShape(Scene *_scene, std::string _name, const QVector3D *_data, int _size)
{
    if(_scene->m_ShapePool[_name])
    {
        qDebug()<<_name.data()<<" already exists in ShapePool";
        return;
    }
    // create new smart pointer + instanciate new Shape
    auto pShape = std::make_shared<Shape>();
    // allocate data on the GPU
    pShape->allocate(_data, _size);
    // add Shape to Pool
    _scene->m_ShapePool[_name] = pShape;
}

void Scene::addModel(Scene *_scene, std::string _name, std::string _path)
{
    if(_scene->m_ModelPool[_name])
    {
        qDebug()<<_name.data()<<" already exists in ModelPool";
        return;
    }
    auto pModel = std::make_shared<Model>();

    // load the model using assimp
     pModel->loadModel(_path);

    _scene->m_ModelPool[_name] = pModel;
}

pSceneOb Scene::addSceneObject(std::string _shape)
{
    pSceneOb newPtr= addSceneObject(_shape, QVector3D(0.0f, 0.0f, 0.0f), QQuaternion::fromAxisAndAngle(0,1,0,0));
    if(newPtr == NULL){qDebug()<<"ptr to NULL returned";};
    return newPtr;
}

pSceneOb Scene::addSceneObject(std::string _shape, const QVector3D &_pos)
{
    pSceneOb newPtr= addSceneObject(_shape, _pos, QQuaternion::fromAxisAndAngle(0,1,0,0));
    if(newPtr == NULL){qDebug()<<"ptr to NULL returned";};
    return newPtr;
}

pSceneOb Scene::addSceneObject(std::string _shape, const QVector3D &_pos, const QQuaternion &_rot)
{
    auto pShape = getShapeFromPool(_shape);
    if(pShape == NULL)
    {
        qDebug()<<"WARNING: COULD NOT ADD SceneObject";
        return NULL;
    }
    auto pSO = std::make_shared<SceneObject>(this, pShape, _pos, _rot);
    m_SceneObjects.push_back(pSO);
    return pSO;
}

float Scene::randf(int _mod)
{
    float r10 = std::rand() % _mod;
    float r1 = (std::rand() % _mod) / 10.0;
    if(r10 == 0.0) r10 = 1;
    if(r10 == 0.0) r1 = 0.1;

//    qDebug()<<"randf is: "<< r10<<"rand1 is: "<< r1 <<" result: "<<r10 * r1;
    return r10 * r1;
}

QVector3D Scene::randVec3(int _mod)
{
    QVector3D randomVec = QVector3D(randf(_mod), randf(_mod), randf(_mod));
//    qDebug()<<"randVec3 is: "<<randomVec;
    return randomVec;
}

float Scene::randfinRange(int a, int b)
{
    float r1 = a + (std::rand() % ( b - a + 1));
    float r2 = (a + (std::rand() % ( b - a + 1))) * 0.1;
//    qDebug()<<r1<<" : "<<r2;
    return r1 + r2;
}

std::shared_ptr<Shape> Scene::getShapeFromPool(std::string _key)
{
    ShapeMap::const_iterator got = m_ShapePool.find(_key);
    if ( got == m_ShapePool.end())
    {
        qDebug()<< "WARNING: NO SHAPE WERE FOUND";
    }
    else
    {
        auto _pShape = got->second;
        return _pShape;
    }
    return 0;
}

std::shared_ptr<Model> Scene::getShapeFromModelPool(std::string _key)
{
    ModelMap::const_iterator got = m_ModelPool.find(_key);
    if ( got == m_ModelPool.end())
    {
        qDebug()<< "WARNING: NO SHAPE WERE FOUND";
    }
    else
    {
        auto _pShape = got->second;
        return _pShape;
    }
    return nullptr;
}

void Scene::bar()
{
    qDebug()<<"bar";
}


void Scene::QtOpenGLinitialize()
{
//    glEnable(GL_CULL_FACE);
        glDisable(GL_CULL_FACE);
//    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    m_arcCamera.translate(0.0f, 0.0f, 6.0f);
    m_arcCamera.SetWorldPos(QVector3D(0.0f, 0.0f, 6.0f));
    m_arcCamera.SetPivot(QVector3D(0.0f, 0.0f, 0.0f));
    m_arcCamera.SetPivotToCam(QVector3D(0,0,6));
    m_arcCamera.arcBallStart();

//----build, compline and link shaders
    m_screen_program = new QOpenGLShaderProgram;
    m_screen_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/screen.vert");
    m_screen_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/screen.frag");
    m_screen_program->link();

    m_lighting_program = new QOpenGLShaderProgram();
    m_lighting_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/tmp.vert");
    m_lighting_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/tmp.frag");
    m_lighting_program->link();

    m_flat_program = new QOpenGLShaderProgram();
    m_flat_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/flat.vert");
    m_flat_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/flat.frag");
    m_flat_program->link();


// sphere buffer creation
    m_sphere_vao = new QOpenGLVertexArrayObject();
    m_sphere_vao->create();
    m_sphere_vao->bind();

    m_sphere_vbo.create();    
    m_sphere_vbo.bind();
    m_sphere_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_sphere_vbo.allocate(vdata, sizeof(vdata));

    m_sphere_ebo = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    m_sphere_ebo.create();
    m_sphere_ebo.bind();
    m_sphere_ebo.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    m_sphere_ebo.allocate(tindices, sizeof(tindices));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(Vertex),
                          nullptr);

     m_sphere_vao->release();
//    m_flat_program->bind();

//    MAKE MODEL TO RENDER
   pModel = std::make_shared<Model>(this, m_flat_program);
//   pModel->loadModel("resources/objects/rock/3Cube_gn.obj");
    pModel->loadModel("resources/objects/nanosuit/nanosuit.obj");
   m_ModelPool["model"] = pModel;

   auto indices = pModel->meshes[0].indices;
   auto vertices = pModel->meshes[0].vertices;

//   qDebug()<<"sizeOf Vert"<<sizeof(unsigned int);
//   qDebug()<<"sizeOf Vert"<<sizeof(vertices[0]);
//   qDebug()<<"size vector"<<vertices.size();
//   qDebug()<<"size IndexVector"<<indices.size();




    m_CubeModel_vao = new QOpenGLVertexArrayObject(window());
    m_CubeModel_vao->create();
    m_CubeModel_vao->bind();

    m_CubeModel_vbo.create();
    m_CubeModel_vbo.bind();
    m_CubeModel_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_CubeModel_vbo.allocate(vertices.data(), vertices.size() * 24);

    m_CubeModel_ebo = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    m_CubeModel_ebo.create();
    m_CubeModel_ebo.bind();
    m_CubeModel_ebo.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    m_CubeModel_ebo.allocate(indices.data(), indices.size()*4);


//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0,
//                          3,
//                          GL_FLOAT,
//                          GL_FALSE,
//                          sizeof(Vertex),
//                          nullptr);

    m_flat_program->enableAttributeArray(0);
    m_flat_program->setAttributeBuffer(
                            0,                     // shader location
                            GL_FLOAT,             // type of elements
                            0,                    // attr offset
                            3,                 // components per vertex attr
                            sizeof(Vertex));               // stride - size of all buffer attrs together

//        m_flat_program->enableAttributeArray(0);
//        m_flat_program->setAttributeBuffer(
//                                0,                     // shader location
//                                GL_FLOAT,             // type of elements
//                                0,                    // attr offset
//                                3);                // components per vertex attr

    // before unbinding EBO do


    m_flat_program->release();

// interpret new Model layout
//    m_lighting_program->enableAttributeArray(0);
//        m_lighting_program->setAttributeBuffer(
//                                0,                     // shader location
//                                GL_FLOAT,             // type of elements
//                                0,                    // attr offset
//                                3,                    // components per vertex attr
//                                sizeof(Vertex)); // stride - size of all buffer attrs together

//    m_flat_program->enableAttributeArray(0);
//    m_flat_program->setAttributeBuffer(
//                            0,                     // shader location
//                            GL_FLOAT,             // type of elements
//                            0,                    // attr offset
//                            3,                    // components per vertex attr
//                            sizeof(Vertex));
//    qDebug()<<sizeof(Vertex);

//    Scene::addShape(this, "cubeLit", &myShapeNormals[0], sizeof(myShapeNormals));
    /* GL equivalent
     * glVertexAttribPointer(
     *                   0,                  // index - shader location
     *                   3,                 //  number of components per generic vertex attribute
     *                   GL_FLOAT,          //  data type of each component
     *                   GL_FALSE,          //  bool to fixed-point data values should be normalized (true)
     *                   6 * sizeof(float), //  stride -byte offset between consecutive generic vertex attributes
     *                   (void*)0));        //  pointer to the first component of data
     */
// ---INterpret CUbe-----
//    m_lighting_program->enableAttributeArray(0);
//    m_lighting_program->setAttributeBuffer(
//                            0,                     // shader location
//                            GL_FLOAT,             // type of elements
//                            0,                    // attr offset
//                            3,                    // components per vertex attr
//                            3*sizeof(QVector3D)); // stride - size of all buffer attrs together
//    m_lighting_program->enableAttributeArray(1);
//    m_lighting_program->setAttributeBuffer(1, GL_FLOAT, sizeof(QVector3D), 3, 3*sizeof(QVector3D));
//    m_lighting_program->enableAttributeArray(2);
//    m_lighting_program->setAttributeBuffer(2, GL_FLOAT, 2*sizeof(QVector3D), 3, 3*sizeof(QVector3D));


    // interpret myCubeNormals
//    m_flat_program->enableAttributeArray(0);
//    m_flat_program->setAttributeBuffer(
//                            0,                     // shader location
//                            GL_FLOAT,             // type of elements
//                            0,                    // attr offset
//                            3,                    // components per vertex attr
//                            3*sizeof(QVector3D));

//----prepare a QuadPlane
    m_quad_vao = new QOpenGLVertexArrayObject(window());
    m_quad_vao->create();
    m_quad_vbo.create();
    m_quad_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_quad_vao->bind();
    m_quad_vbo.bind();
    m_quad_vbo.allocate(quad, 30 * sizeof(GLfloat));
    // tell simple shader how to interpret the quadPlane
    m_screen_program->setAttributeBuffer("position", GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
    m_screen_program->enableAttributeArray("position");
    m_screen_program->setAttributeBuffer("uv", GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));
    m_screen_program->enableAttributeArray("uv");
    m_quad_vbo.release();
    m_quad_vao->release();

    //----Fill SceneObjects array
    // create a framebuffer
    m_gbuffer_fbo = new QOpenGLFramebufferObject(window()->width()*2, window()->height()*2);
    m_gbuffer_fbo->bind();
    m_gbuffer_fbo->addColorAttachment(window()->width()*2, window()->height()*2, GL_RGB);

    // create a texture the fb can render to
    m_view_position_texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    m_view_position_texture->setSize(window()->width()*2, window()->height()*2);
    m_view_position_texture->setMinificationFilter(QOpenGLTexture::Nearest);
    m_view_position_texture->setMagnificationFilter(QOpenGLTexture::Nearest);
    m_view_position_texture->setFormat(QOpenGLTexture::RGB32F);
    m_view_position_texture->allocateStorage(QOpenGLTexture::RGB, QOpenGLTexture::Float32);

    //bind the texture to the framebuffer
    glBindTexture(GL_TEXTURE_2D, m_view_position_texture->textureId());
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_view_position_texture->textureId(), 0);

    //create a render buffer
    GLuint rbo_depth;
    glGenRenderbuffers(1, &rbo_depth);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth);
    glRenderbufferStorage(
      GL_RENDERBUFFER,     // Target
      GL_DEPTH_COMPONENT,  // Internal Format
      window()->width()*2, window()->height()*2    // Dimensions
    );
    glFramebufferRenderbuffer(
      GL_FRAMEBUFFER,       // Target
      GL_DEPTH_ATTACHMENT,  // Attachment
      GL_RENDERBUFFER,      // Renderbuffer Target
      rbo_depth             // Renderbuffer
    );
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      qCritical("gBuffer FBO not complete!");

    m_gbuffer_fbo->release();

//    SceneInitialize();
    initLights();
    //printVersionInformation();
}

void Scene::paint()
{
    // draw to the framebuffer (off-screen render)
    glViewport(0,0, m_gbuffer_fbo->width(), m_gbuffer_fbo->height());
    glDisable(GL_CULL_FACE);

    m_gbuffer_fbo->bind();
      glEnable(GL_DEPTH_TEST);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glClearColor(0.2f, 0.0f, 0.0f, 1.0f);

      m_lighting_program->bind();
      m_lighting_program->setUniformValue("ProjectionMatrix", m_projection_matrix);
      m_lighting_program->setUniformValue("ViewMatrix", m_arcCamera.toMatrix());
      m_lighting_program->setUniformValue("viewPos", m_arcCamera.worldPos());

      // set light properties for single pointLight (doent exist on client side)
      m_lighting_program->setUniformValue("objectColor", 1.0f, 0.5f, 0.31f);
      m_lighting_program->setUniformValue("lightColor", 1.0f, 1.0f, 1.0f);
//      m_lighting_program->setUniformValue("lightPos", m_SceneObjects[0]->getPos());
       m_lighting_program->setUniformValue("lightPos", QVector3D(0,0,0));

       //------------------------draw Model-----------------------------------------------------------------------------
 //      pModel->meshes[0].draw();
//       pModel->meshes[0].m_vao.bind();
//       glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
//       pModel->meshes[0].m_vao.release();

       m_lighting_program->release();
       //------------------------end draw Model-----------------------------------------------------------------------------



//       ------------------test sphere Code--------------------------------------------------------------------------
      m_flat_program->bind();
      m_flat_program->setUniformValue("ProjectionMatrix", m_projection_matrix);
      m_flat_program->setUniformValue("ViewMatrix", m_arcCamera.toMatrix());


//      m_sphere_vao->bind();
//      m_sphere_Mmatrix.setToIdentity();
//      m_flat_program->setUniformValue("ModelMatrix", m_sphere_Mmatrix);
////      glDrawArrays(GL_TRIANGLES, 0, 120);
//      glDrawElements(GL_TRIANGLES, 59, GL_UNSIGNED_INT, 0);
//      m_sphere_vao->release();

//      m_CubeModel_vao->bind();
//////      glBindVertexArray( m_CubeModel_vao->objectId());
//////      glBindVertexArray(pModel->meshes[0].m_pVao->objectId());
//      m_sphere_Mmatrix.setToIdentity();
//      m_sphere_Mmatrix.translate(QVector3D(0,-2,0));
//      m_flat_program->setUniformValue("ModelMatrix", m_sphere_Mmatrix);
//      glDrawElements(GL_TRIANGLES, 59, GL_UNSIGNED_INT, 0);
//      m_CubeModel_vao->release();
//      glBindVertexArray( 0);

//      pModel->bind();
//      pModel->meshes[0].m_pVao->bind();
//      m_sphere_Mmatrix.setToIdentity();
//      m_flat_program->setUniformValue("ModelMatrix", m_sphere_Mmatrix);
////      glDrawElements(GL_LINES, 59, GL_UNSIGNED_INT, 0);
//      glDrawElements(GL_TRIANGLES, 59, GL_UNSIGNED_INT, 0);
//      pModel->meshes[0].m_pVao->release();

      m_sphere_Mmatrix.setToIdentity();
      m_flat_program->setUniformValue("ModelMatrix", m_sphere_Mmatrix);
      pModel->draw();

//      auto id1 = pModel->meshes[0].m_pVao->objectId();
//      auto id2 = m_CubeModel_vao->objectId();

//      m_CubeModel_vao->bind();
//      m_sphere_Mmatrix.setToIdentity();
//      m_flat_program->setUniformValue("ModelMatrix", m_sphere_Mmatrix);
//      glDrawElements(GL_LINES, 59, GL_UNSIGNED_INT, 0);
//      m_CubeModel_vao->release();

      m_flat_program->release();
//       -------------------end Sphere Code------------------------------------------------------------------------------


    // set light properties for single Material (doent exist on client side)
      m_lighting_program->setUniformValue("material.diffuse", QVector3D(1.0f, 0.0f, 0.0f));
      m_lighting_program->setUniformValue("material.ambient", QVector3D(1.0f, 0.1f, 0.1f));
      m_lighting_program->setUniformValue("material.specular", QVector3D(0.0f, 1.0f, 0.0f));
      m_lighting_program->setUniformValue("material.shininess", 20);

//       set light uniforms
      for(uint i = 0; i < m_lights.size(); i++)
      {
          std::string uniFName = "dirLights[" + std::to_string(i) +"]";
          m_lighting_program->setUniformValue((uniFName+".direction").c_str(), m_lights[i].direction);
          m_lighting_program->setUniformValue((uniFName+".specular").c_str(), m_lights[i].specular);
          m_lighting_program->setUniformValue((uniFName+".diffuse").c_str(), m_lights[i].diffuse);
          m_lighting_program->setUniformValue((uniFName+".ambient").c_str(), m_lights[i].ambient);
      }

      // draw cube with phong shader
//      for(uint i = 1; i < m_SceneObjects.size(); i++)
//      {
//        m_SceneObjects[i]->bind();
//        m_lighting_program->setUniformValue("ModelMatrix", m_SceneObjects[i]->getMatrix());
//        glDrawArrays(GL_TRIANGLES, 0, m_SceneObjects[i]->shape()->getVertsSize() /
//                     sizeof(m_SceneObjects[i]->shape()->getData()[0]));
//        m_SceneObjects[i]->release();
//      }
//      m_lighting_program->release();

      // draw light with flat shader
//      m_flat_program->bind();
//      m_flat_program->setUniformValue("ProjectionMatrix", m_projection_matrix);
//      m_flat_program->setUniformValue("ViewMatrix", m_arcCamera.toMatrix());
//      m_SceneObjects[0]->bind();
////      m_SceneObjects[0]->setTranslation(m_arcCamera.pivot());
////      m_SceneObjects[0]->setTranslation(QVector3D(0,2,0));
//      m_flat_program->setUniformValue("ModelMatrix", m_SceneObjects[0]->getMatrix());
//      glDrawArrays(GL_TRIANGLES, 0, m_SceneObjects[0]->shape()->getVertsSize() /
//                   sizeof(m_SceneObjects[0]->shape()->getData()[0]));
//      m_SceneObjects[0]->release();
//      m_flat_program->release();

//      m_lighting_program->release();
    m_gbuffer_fbo->release();

    // draw to screen
    glViewport ( 0, 0, window()->width()*2, window()->height()*2);
    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // finally draw a quad to the screen
    m_screen_program->bind();
    m_view_position_texture->bind(0);
    m_quad_vao->bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    m_quad_vao->release();
    m_screen_program->release();
}

void Scene::update()
{
    // update from Window Qtimer animate cube
    //m_myTransform.rotate(1.0f, QVector3D(0.4f, 0.3f, 0.3f));
}

void Scene::SceneInitialize()
{

    auto pSO1 = addSceneObject("cubeLit");
    float rf= randf();
    pSO1->setScale(QVector3D(0.3, 0.3, 0.3));

    pSO1 = addSceneObject("cubeLit", QVector3D(0,0,0));
    rf= randf(5);
    pSO1->setScale(QVector3D(10.0, 10.0, 10.0));

//    int num = 20;
//    for(int i = 0; i<num ; i++)
//    {
//        pSO1 = addSceneObject("cubeLit", randVec3(i % 5 + 8));
//        rf= randf(5);
//        pSO1->setScale(QVector3D(rf, rf, rf));
//        float x,y,z, w;
//        x = randfinRange(-1,1);
//        y = randfinRange(-1,1);
//        z = randfinRange(-1,1);
//        w = randfinRange(90,90);
//        pSO1->rotate(QQuaternion::fromAxisAndAngle(x, y, z, w));
//    }
}

void Scene::initLights()
{
    // init simple 3-point DirLight setup
    Light lightA, lightB, lightC;

    lightA.direction = QVector3D(0.0, -1.0, 0.0);
    lightA.ambient = QVector3D(0.1, 0.1, 0.1);
    lightA.diffuse = QVector3D(1.0, 0.0, 0.0);
    lightA.specular = QVector3D(0.4, 0.4, 0.4);

//    lightB.direction = QVector3D(1.0, 0.8, 0.2);
//    lightB.ambient = QVector3D(0.2, 0.2, 0.2);
//    lightB.diffuse = QVector3D(1.0, 1.0, 1.0);
//    lightB.specular = QVector3D(0.0, 1.0, 0.0);

//    lightC.direction = QVector3D(-0.3, 0.5, 1.0);
//    lightC.ambient = QVector3D(0.0, 0.0, 0.0);
//    lightC.diffuse = QVector3D(0.0, 0.0, 0.0);
//    lightC.specular = QVector3D(0.1, 0.1, 0.4);

    m_lights.push_back(lightA);
//    m_lights.push_back(lightB);
//    m_lights.push_back(lightB);
}





























