#include <QFile>
#include <QString>

#include "Scene.h"


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

ModelPtr Scene::addModel(Scene *_scene, std::string _name, std::string _path)
{
    if(_scene->m_ModelPool[_name])
    {
        qDebug()<<_name.data()<<" already exists in ModelPool";
        return nullptr;
    }
    auto pModel = std::make_shared<Model>();
    // load the model using assimp
    pModel->loadModel(_path);
    m_ModelPool[_name] = pModel;
    return pModel;
}

pSceneOb Scene::addSceneObject(std::string _shape)
{
    pSceneOb newPtr= addSceneObject(_shape, QVector3D(0.0f, 0.0f, 0.0f), QQuaternion::fromAxisAndAngle(0,1,0,0));
    if(newPtr == nullptr){qDebug()<<"ptr to NULL returned";};
    return newPtr;
}

pSceneOb Scene::addSceneObject(std::string _shape, const QVector3D &_pos)
{
    pSceneOb newPtr= addSceneObject(_shape, _pos, QQuaternion::fromAxisAndAngle(0,1,0,0));
    if(newPtr == nullptr){qDebug()<<"ptr to NULL returned";};
    return newPtr;
}

pSceneOb Scene::addSceneObject(std::string _name, const QVector3D &_pos, const QQuaternion &_rot)
{

        auto pShape = getShapeFromPool(_name);
        if(pShape == nullptr)
        {
            qDebug()<<"WARNING: COULD NOT ADD SceneObject";
            return nullptr;
        }
        auto pSO = std::make_shared<SceneObject>(this, pShape, _pos, _rot);
        m_SceneObjects.push_back(pSO);
        return pSO;
}

pSceneOb Scene::addSceneObjectFromModel(std::string _name, const QVector3D &_pos, const QQuaternion &_rot)
{
    auto pModel = getModelFromPool(_name);
    if(pModel == nullptr)
    {
        qDebug()<<"WARNING: COULD NOT ADD SceneObject";
        return nullptr;
    }
    auto pSO = std::make_shared<SceneObject>(this, pModel, _pos, _rot);
    m_SceneObjects.push_back(pSO);
    return pSO;
}

LightPtr Scene::addPointLight(const QVector3D &_pos, const QVector3D &_color)
{
    auto pLight = std::make_shared<Light>();
    pLight->position = _pos;
    pLight->color = _color;
    m_Pointlights.push_back(pLight);
    return pLight;
}

MaterialPtr Scene::addMaterial(const QVector3D &_ambient, const QVector3D &_diffuse, const QVector3D &_specular, float _shininess)
{
    auto pMaterial = std::make_shared<Material>();
    pMaterial->ambient = _ambient;
    pMaterial->diffuse = _diffuse;
    pMaterial->specular = _specular;
    pMaterial->shininess = _shininess;
    m_Materials.push_back(pMaterial);
    return pMaterial;
}

ShapePtr Scene::getShapeFromPool(std::string _key)
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
    return nullptr;
}

ModelPtr Scene::getModelFromPool(std::string _key)
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

void Scene::QtOpenGLinitialize()
{
    glEnable(GL_CULL_FACE);
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


    addPointLight();

    addMaterial(    QVector3D(0.01f , 0.04f ,0.1f),   // ambient
                    QVector3D(0.1f , 0.3f ,0.8f),    // diffuse
                    QVector3D(1.0f , 1.0f ,1.0f),    // specular
                    32.0 );                          // shininess

    addMaterial(    QVector3D(0.02f , 0.0f ,0.0f),   // ambient
                    QVector3D(0.5f , 0.05f ,0.05f),  // diffuse
                    QVector3D(1.0f , 1.0f ,1.0f),    // specular
                    20.0 );                          // shininess

//    MAKE MODEL TO RENDER
   addModel(this, "nanoSuit", "resources/objects/nanosuit.obj");

   addModel(this, "bunny", "../bunny.obj");

   addShape(this, "Cube", &CubeWithNormals[0], sizeof(CubeWithNormals));

   addSceneObject("Cube");
   addSceneObject("Cube");

   m_flat_program->enableAttributeArray(0);
   m_flat_program->setAttributeBuffer(
                           0,                     // shader location
                           GL_FLOAT,             // type of elements
                           0,                    // attr offset
                           3,                    // components per vertex attr
                           3*sizeof(QVector3D)); // stride - size of all buffer attrs together


   addSceneObjectFromModel("nanoSuit", QVector3D(0,0,0), QQuaternion(1,0,0,0));
   addSceneObjectFromModel("bunny", QVector3D(0,0,0), QQuaternion(1,0,0,0));

   m_SceneObjects[0]->setScale(QVector3D(0.2, 0.2, 0.2));
   m_SceneObjects[2]->setScale(QVector3D(0.2, 0.2, 0.2));
   m_SceneObjects[2]->translate(QVector3D(-2,0,0));
   m_SceneObjects[3]->setScale(QVector3D(10, 10, 10));
   m_SceneObjects[3]->translate(QVector3D(2,0,0));


   m_flat_program->setAttributeBuffer(
                            0,                     // shader location
                            GL_FLOAT,             // type of elements
                            0,                    // attr offset
                            3,                    // components per vertex attr
                            3*sizeof(QVector3D));

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

//      m_lighting_program->setUniformValue("ModelMatrix",  m_SceneObjects[1]->getMatrix());

      // set light properties for single pointLight (doent exist on client side)
      if(m_SceneObjects[0])
            m_lighting_program->setUniformValue("lightPos",  m_SceneObjects[0]->getPos());
      else
          m_lighting_program->setUniformValue("lightPos", QVector3D(0,4,2) );

      m_lighting_program->setUniformValue("lightColor", 1.0f, 1.0f, 1.0f);
      m_lighting_program->setUniformValue("viewPos", m_arcCamera.worldPos());
      m_lighting_program->setUniformValue("objectColor", 1.0f, 0.5f, 0.31f);

       m_lighting_program->setUniformValue("ModelMatrix",  m_SceneObjects[2]->getMatrix());
       m_SceneObjects[2]->draw();
        m_lighting_program->setUniformValue("ModelMatrix",  m_SceneObjects[3]->getMatrix());
       m_SceneObjects[3]->draw();


 // set light uniforms loop
 //      for(uint i = 0; i < m_lights.size(); i++)
 //      {
 //          std::string uniFName = "dirLights[" + std::to_string(i) +"]";
 //          m_lighting_program->setUniformValue((uniFName+".direction").c_str(), m_lights[i].direction);
 //          m_lighting_program->setUniformValue((uniFName+".specular").c_str(), m_lights[i].specular);
 //          m_lighting_program->setUniformValue((uniFName+".diffuse").c_str(), m_lights[i].diffuse);
 //          m_lighting_program->setUniformValue((uniFName+".ambient").c_str(), m_lights[i].ambient);
 //      }


       m_lighting_program->release();

//-------------------------Draw flatShader Cube---------------------------------------------------------------------------
      m_flat_program->bind();
      m_flat_program->setUniformValue("ProjectionMatrix", m_projection_matrix);
      m_flat_program->setUniformValue("ViewMatrix", m_arcCamera.toMatrix());
      m_flat_program->setUniformValue("ModelMatrix", m_SceneObjects[0]->getMatrix());

      m_SceneObjects[0]->draw();
//      m_flat_program->setUniformValue("ModelMatrix", m_SceneObjects[1]->getMatrix());
//      m_SceneObjects[1]->draw();


      m_flat_program->release();
//       -------------------end Sphere Code------------------------------------------------------------------------------
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

}

void Scene::initLights()
{
    // init simple 3-point DirLight setup

}





























