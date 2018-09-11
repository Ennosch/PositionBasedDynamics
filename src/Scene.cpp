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


    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, window()->width()*2, window()->height()*2, GL_TRUE);

    glBindTexture(GL_TEXTURE_2D, screenTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window()->width()*2, window()->height()*2, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);


    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
//    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, window()->width()*2, window()->height()*2);
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

/*
 *
 * // WIP
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
*/

pSceneOb Scene::addSceneObjectFromModel(std::string _name, const uint _materialID,  const QVector3D &_pos, const QQuaternion &_rot)
{
    auto pModel = getModelFromPool(_name);
    if(pModel == nullptr)
    {
        qDebug()<<"WARNING: COULD NOT ADD SceneObject";
        return nullptr;
    }
    auto pSO = std::make_shared<SceneObject>(this, pModel, _materialID ,_pos, _rot);
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

void Scene::QtOpenGLinitialize_backup()
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
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



//---------------------frame buffer pain ---------------------
//    QOpenGLFramebufferObjectFormat muliSampleFormat;
//    muliSampleFormat.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
//    muliSampleFormat.setMipmap(true);
//    muliSampleFormat.setSamples(4);
//    muliSampleFormat.setTextureTarget(GL_TEXTURE_2D);
//    muliSampleFormat.setInternalTextureFormat(GL_RGBA32F_ARB);

//    m_fbo_A= new QOpenGLFramebufferObject(window()->width()*2, window()->height()*2, muliSampleFormat);

//    QOpenGLFramebufferObjectFormat downSampledFormat;
//    downSampledFormat.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
//    downSampledFormat.setMipmap(true);
//    downSampledFormat.setTextureTarget(GL_TEXTURE_2D);
//    downSampledFormat.setInternalTextureFormat(GL_RGBA32F_ARB);

//    m_fbo_B = new QOpenGLFramebufferObject(window()->width()*2, window()->height()*2, downSampledFormat);


    // create a framebuffer
    QOpenGLFramebufferObjectFormat _testFBOFormat;
    _testFBOFormat.setSamples(4);
    _testFBOFormat.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    _testFBOFormat.setTextureTarget(GL_TEXTURE_2D_MULTISAMPLE);

//    m_gbuffer_fbo = new QOpenGLFramebufferObject(window()->width()*2, window()->height()*2, _testFBOFormat);
    m_gbuffer_fbo = new QOpenGLFramebufferObject(window()->width()*2, window()->height()*2);
    m_gbuffer_fbo->bind();
    // Creates and attaches an additional texture or renderbuffer of size width and height.
//    QOpenGLFramebufferObject::blitFramebuffer( m_fbo_B , m_gbuffer_fbo , GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT| GL_STENCIL_BUFFER_BIT,GL_NEAREST);
    // create a texture the fb can render to

    ////  for multisample
    m_view_position_texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
//    m_view_position_texture = new QOpenGLTexture(QOpenGLTexture::Target2DMultisample);
    m_view_position_texture->create();
    m_view_position_texture->bind();
//    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_view_position_texture->textureId());
//    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, window()->width()*2, window()->height()*2, GL_TRUE);
    m_view_position_texture->setSize(window()->width()*2, window()->height()*2);
    m_view_position_texture->setMinificationFilter(QOpenGLTexture::Nearest);
    m_view_position_texture->setMagnificationFilter(QOpenGLTexture::Nearest);
    // change to GL_DEPTH24_STENCIL8 fixed 36182 => GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE, a wrong move the texture should be the color buffer hence D24S8 worng and back to RGB32F
    // the teture is a color buffer. Depth and Stencil is wrong here
    m_view_position_texture->setFormat(QOpenGLTexture::RGB32F);
//    m_view_position_texture->setSamples(4);
    //DepthStencil
    m_view_position_texture->allocateStorage(QOpenGLTexture::RGB, QOpenGLTexture::Float32);
    m_gbuffer_fbo->addColorAttachment(window()->width()*2, window()->height()*2, GL_RGB);

//     qDebug()<<"texture ID:"<<m_view_position_texture->textureId();

    // check here if my custom fb is bound

//    int _glValue;
//    glGetIntegerv(GL_ACTIVE_PROGRAM, &_glValue);
//    qDebug()<<"logGL:"<<unsigned(_glValue);


    // attach texture to fbo
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_view_position_texture->textureId(), 0);
//    glFramebufferTexture2D(GL_FRAMEBUFFER,                      // target     (our currently bound custom fbo)
//                           GL_COLOR_ATTACHMENT0,                // attachment (can be more than 1 color)
//                           GL_TEXTURE_2D_MULTISAMPLE,           // textarget type of texture
//                           m_view_position_texture->textureId(),// texture (ID)
//                           0);                                  // level (mipmap level)

//    auto whatIsAttatched = m_gbuffer_fbo->takeTexture(2);
//    qDebug()<<"take texture:"<<whatIsAttatched;

    m_view_position_texture->release();

// ignore depth rbo for now

    //create a render buffer (rbo) serving as DepthStencil buffer
    GLuint rbo_depth;
    glGenRenderbuffers(1, &rbo_depth);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth);
    //  for multisample
    glRenderbufferStorage(
      GL_RENDERBUFFER,     // Target
      GL_DEPTH_COMPONENT,  // Internal Format (?) check GL_DEPTH24_STENCIL8  here
      window()->width()*2, window()->height()*2    // Dimensions
    );
//    glRenderbufferStorageMultisample(
//      GL_RENDERBUFFER,     // Target
//                4,         // probably sample count
//      GL_DEPTH24_STENCIL8,  // Internal Format
//      window()->width()*2, window()->height()*2    // Dimensions
//    );
    // attach rbo to fbo
    glFramebufferRenderbuffer(
      GL_FRAMEBUFFER,       // Target
      GL_DEPTH_ATTACHMENT,  // Attachment
      GL_RENDERBUFFER,      // Renderbuffer Target
      rbo_depth             // Renderbuffer
    );
        glBindRenderbuffer(GL_RENDERBUFFER, 0);


    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      qCritical()<<"gBuffer FBO not complete! error enum:"<< (glCheckFramebufferStatus(GL_FRAMEBUFFER));

//    auto oglEnum = GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE;
//    auto oglEnum = GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT;
//    qDebug()<<oglEnum;

    /*
     *36054 GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT is returned if any of the framebuffer attachment points are framebuffer incomplete.
     *
     * 36182:
     *
     * value of GL_RENDERBUFFER_SAMPLES is not the same for all attached renderbuffers
     * value of GL_TEXTURE_SAMPLES is the not same for all attached textures
     * or
     * attached images are a mix of renderbuffers and textures
     * value of GL_RENDERBUFFER_SAMPLES does not match the value of GL_TEXTURE_SAMPLES
     *
     * also
     * value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not the same for all attached textures
     * or
     * attached images are a mix of renderbuffers and textures
     * value of GL_TEXTURE_FIXED_SAMPLE_LOCATIONS is not GL_TRUE for all attached textures
     */

    m_gbuffer_fbo->release();

    setupScene();
}

void Scene::QtOpenGLinitialize()
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
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

    //--------manual Frame Buffer workflow--------------
    SCR_WIDTH = window()->width()*2;
    SCR_HEIGHT = window()->height()*2;

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    //-------------texture as color buffer
    glGenTextures(1, &texture);
//    glBindTexture(GL_TEXTURE_2D, texture);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, window()->width()*2, window()->height()*2, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

    // attach multisampled texture
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, texture, 0);

//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    // ------------rbo for depth as stencil buffer------------------
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
//    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, window()->width()*2, window()->height()*2);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      qCritical()<<"gBuffer FBO not complete! error enum:"<< (glCheckFramebufferStatus(GL_FRAMEBUFFER));
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //------------------fbo end-------------------

    intermediateFBO;
    glGenFramebuffers(1, &intermediateFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);

    glGenTextures(1, &screenTexture);
    glBindTexture(GL_TEXTURE_2D, screenTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window()->width()*2, window()->height()*2, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      qCritical()<<"gBuffer FBO not complete! error enum:"<< (glCheckFramebufferStatus(GL_FRAMEBUFFER));
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

//    m_screen_program->setUniformValue("screenTexture", 0);

    GLuint id = m_screen_program->programId();
    glUniform1i(glGetUniformLocation(id, "screenTexture"), 0);

    setupScene();
}

void Scene::paint()
{
    // draw to the framebuffer (off-screen render)
    glViewport(0,0, window()->width()*2, window()->height()*2);
//    glViewport(0,0, SCR_WIDTH, SCR_HEIGHT);
    glDisable(GL_CULL_FACE);
    // bind old QtWrapper style
    // m_gbuffer_fbo->bind();
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

      glEnable(GL_DEPTH_TEST);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

              m_lighting_program->bind();
              m_lighting_program->setUniformValue("ProjectionMatrix", m_projection_matrix);
              m_lighting_program->setUniformValue("ViewMatrix", m_arcCamera.toMatrix());
              m_lighting_program->setUniformValue("viewPos", m_arcCamera.worldPos());
              m_lighting_program->setUniformValue("numPointLights", int(m_Pointlights.size()));
              m_lighting_program->setUniformValue("numMaterials", int(m_Materials.size()));

              // set Position + color for all pointsLights
              for(uint i = 0; i < m_Pointlights.size(); i++)
              {
                    std::string uniFName = "PointLights[" + std::to_string(i) +"]";
                    m_lighting_program->setUniformValue((uniFName+".position").c_str(), m_Pointlights[i]->position);
                    m_lighting_program->setUniformValue((uniFName+".color").c_str(), m_Pointlights[i]->color);
              }

              m_lighting_program->setUniformValue("objectColor", 1.0f, 0.5f, 0.31f);

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
              for(uint i = 1; i < m_SceneObjects.size(); i++)
              {
                  uint matID = m_SceneObjects[i]->getMaterialID();
                  m_lighting_program->setUniformValue("mMaterial.ambient", m_Materials[matID]->ambient );
                  m_lighting_program->setUniformValue("mMaterial.diffuse", m_Materials[matID]->diffuse );
                  m_lighting_program->setUniformValue("mMaterial.specular", m_Materials[matID]->specular );
                  m_lighting_program->setUniformValue("mMaterial.shininess", m_Materials[matID]->shininess );
                  m_lighting_program->setUniformValue("ModelMatrix",  m_SceneObjects[i]->getMatrix());
                  m_SceneObjects[i]->draw();
              }

              // slow maker
//              for(uint i = 1; i < 100000; i++)
//              {
//                  uint matID = m_SceneObjects[1]->getMaterialID();
//                  m_lighting_program->setUniformValue("mMaterial.ambient", m_Materials[matID]->ambient );
//                  m_lighting_program->setUniformValue("mMaterial.diffuse", m_Materials[matID]->diffuse );
//                  m_lighting_program->setUniformValue("mMaterial.specular", m_Materials[matID]->specular );
//                  m_lighting_program->setUniformValue("mMaterial.shininess", m_Materials[matID]->shininess );
//                  m_lighting_program->setUniformValue("ModelMatrix",  m_SceneObjects[1]->getMatrix());
//                  m_SceneObjects[1]->draw();
//              }


               m_lighting_program->release();

        auto testModel = m_ModelPool["Icosahedron"];

        //-------------------------Draw Wireframe---------------------------------------------------------------------------
              m_flat_program->bind();
              m_flat_program->setUniformValue("ProjectionMatrix", m_projection_matrix);
              m_flat_program->setUniformValue("ViewMatrix", m_arcCamera.toMatrix());
              if(m_SceneObjects[0])
              {
                  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
                  m_flat_program->setUniformValue("ModelMatrix",  m_SceneObjects[0]->getMatrix());
                  m_SceneObjects[0]->draw();
              }
              m_flat_program->release();

    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
    glBlitFramebuffer(0, 0, window()->width()*2, window()->height()*2, 0, 0, window()->width()*2, window()->height()*2, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glViewport ( 0, 0, window()->width()*2, window()->height()*2);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

    m_screen_program->bind();
    m_quad_vao->bind();

//    glBindTexture(GL_TEXTURE_2D, texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, screenTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    m_quad_vao->release();
    m_screen_program->release();

}

void Scene::drawScreenQuad()
{
    glViewport ( 0, 0, window()->width()*2, window()->height()*2);
    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // finally draw a quad to the screen
    m_screen_program->bind();
    m_view_position_texture->bind(0);
    m_quad_vao->bind();
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    m_quad_vao->release();
    m_screen_program->release();
}

void Scene::SceneInitialize()
{

}

void Scene::setupScene()
{
//        addPointLight(QVector3D(5,0,0), QVector3D(0, 0, 1));
//        addPointLight(QVector3D(-5,0,0), QVector3D(0, 1, 0));
//        addPointLight(QVector3D(0,5,0), QVector3D(1, 0, 0));

         addPointLight(QVector3D(0,5,0), QVector3D(1.0, 1.0, 1.0));
         addPointLight(QVector3D(2,5,0), QVector3D(0.6, 0.6, 1.0));
         addPointLight(QVector3D(0,3,2), QVector3D(0.8, 0.8, 0.8));


         // blue tone
        addMaterial(    QVector3D(0.01f , 0.04f ,0.1f),   // ambient
                        QVector3D(0.1f , 0.3f ,0.8f),    // diffuse
                        QVector3D(1.0f , 1.0f ,1.0f),    // specular
                        32.0 );                          // shininess

        // organge tone
        addMaterial(    QVector3D(0.1f , 0.01f ,0.01f),   // ambient
                        QVector3D(0.5f , 0.2f ,0.07f),   // diffuse
                        QVector3D(1.0f , 1.0f ,1.0f),    // specular
                        20.0 );                          // shininess


        // green tone
        addMaterial(    QVector3D(0.0f , 0.0f ,0.0f),   // ambient
                        QVector3D(0.2f , 0.3f ,0.05f),   // diffuse
                        QVector3D(1.0f , 1.0f ,1.0f),    // specular
                        15.0 );

    //    MAKE MODEL TO RENDER
       addModel(this, "grid1", "../Grid1.obj");
       addModel(this, "nanoSuit", "resources/objects/nanosuit.obj");
       addModel(this, "bunny", "../bunny.obj");
       addModel(this, "grid", "../Grid100.obj");

       addModel(this, "HCube", "resources/objects/HCube.obj");
       addModel(this, "Icosahedron", "../Icosahedronf4.obj");
       addShape(this, "Cube", &CubeWithNormals[0], sizeof(CubeWithNormals));

       // ONlY RENDER WITH addSceneObjectFromModel(), otherwise crash (WIP)
       addSceneObjectFromModel("grid", 1, QVector3D(0, 0 ,0 ), QQuaternion(1,0,0,0));
       addSceneObjectFromModel("Icosahedron", 0, QVector3D(0,1,0), QQuaternion(1,0,0,0));
       addSceneObjectFromModel("bunny", 1, QVector3D(1.5 , 0 ,0 ), QQuaternion(1,0,0,0));
       addSceneObjectFromModel("nanoSuit", 2, QVector3D(-1.2,0,0), QQuaternion(1,0,0,0));


       auto myModel = m_ModelPool["grid1"];
}

























