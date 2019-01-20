#include <QFile>
#include <QString>

#include "Scene.h"


int Scene::numCreation = 0;

Scene::Scene(Window *_window) : AbstractScene(_window)
{

}

Scene::Scene(GLWidget *_widget) : AbstractScene(_widget)
{
//    Scene::numCreation = 0;
}

Scene::~Scene()
{

}

void Scene::initialize()
{
  m_CollisionDetect =  CollisionDetection();
  AbstractScene::initialize();

  if (window())
  {
      SCR_WIDTH = window()->width() * 2;
      SCR_HEIGHT = window()->height() * 2;
      qDebug()<<"window"<<SCR_WIDTH<<SCR_HEIGHT;
  }
  else if(widget())
  {
      SCR_WIDTH = widget()->width() * 2;
      SCR_HEIGHT = widget()->height() * 2;
      qDebug()<<"widget"<<SCR_WIDTH<<SCR_HEIGHT;
  }
  else
  {
      SCR_WIDTH = 720;
      SCR_HEIGHT = 720;
  }

  QtOpenGLinitialize();
  DynamicsInitialize();
  setupScene();
  m_DynamicsWorld->generateData();

//  qDebug()<<"=-=-==-start=-=-=-=";

//  QVector3D** ptr = m_DynamicsWorld->m_debugLines.data();

//  QVector3D* ptr2 = *ptr;

//  QVector3D result = *ptr2;



//  QVector3D* test1 = m_DynamicsWorld->m_debugLines[0];
//   QVector3D* test2 = m_DynamicsWorld->m_debugLines[1];

//   QVector3D res1 = *test1;
//   QVector3D res2 = *test2;


//  qDebug()<<"=-=-==-end=-=-=-="<<ptr2<<&res1<<&res2;

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

    // pass them the activeObject instance, so they can notify their observer
    pSO->setActiveObject(widget()->activeObject());
    numCreation++;
    pSO->setID(numCreation);

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

void Scene::addLine(const Line &_line)
{
    m_Lines.push_back(_line);
//    updateLinesVBO();
}

void Scene::addLine(const Ray &_ray, float t=1.0)
{
    Line line;
    line.Start = _ray.Origin;
//    line.End = (_ray.Dir - _ray.Origin) * t;
    line.End = _ray.Origin + (_ray.Dir * t);
    m_Lines.push_back(line);
//    updateLinesVBO();
}

void Scene::addLine(const QVector3D &_start, const QVector3D &_end)
{
    Line line;
    line.Start = _start;
    line.End = _end;
    m_Lines.push_back(line);
//    updateLinesVBO();
}

void Scene::makeDynamic(pSceneOb _sceneObject)
{
    qDebug()<<"make Dynamic";
//    m_DynamicsWorld.addDynamicObject(_sceneObject);
    ParticlePtr newParticle = m_DynamicsWorld->addParticle(_sceneObject->getPos().x(), _sceneObject->getPos().y(), _sceneObject->getPos().z());
    auto newDynamicObject = m_DynamicsWorld->addDynamicObjectAsParticle(_sceneObject, newParticle);
    _sceneObject->makeDynamic(newDynamicObject);
    //    _sceneObject->
}

ShapePtr Scene::getShapeFromPool(std::string _key)
{
    ShapeMap::const_iterator got = m_ShapePool.find(_key);
    if ( got == m_ShapePool.end())
    {
        qDebug()<< "WARNING: NO SHAPE WAS FOUND";
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
        qDebug()<< "WARNING: NO MODEL WAS FOUND";
    }
    else
    {
        auto _pShape = got->second;
        return _pShape;
    }
    return nullptr;
}

pSceneOb Scene::getPointerFromSceneObject(const SceneObject *_sceneObject)
{
//    auto result = std::find(std::begin(m_SceneObjects), std::end(m_SceneObjects), 'must be a sharedPointer but need to compare data')

//    ShapeMap::const_iterator got = m_ShapePool.find(_key);
//    std::vector <pSceneOb>::const_iterator it;


    for (pSceneOb i : m_SceneObjects)
    {
        if(i.get() == _sceneObject)
            return i;
    }

    return nullptr;
}

DynamicsWorld* Scene::dynamicsWorld()
{
    return m_DynamicsWorld;
}

Ray Scene::castRayFromCamera(float ndcX, float ndcY, float depthZ = -1.0)
{
    QVector4D ray_clip = QVector4D(ndcX,ndcY,depthZ, 1);
    QVector4D ray_eye = m_projection_matrix.inverted() * ray_clip;
    ray_eye = QVector4D(ray_eye.x(), ray_eye.y(), -1, 0);
    QVector4D ray_wor = m_arcCamera.toMatrix().inverted() * ray_eye;
    QVector3D ray_world = QVector3D(ray_wor.x(), ray_wor.y(), ray_wor.z());
    QVector3D origin = m_arcCamera.toMatrix().inverted() * QVector3D(0,0,0);

    Ray cameraRay;
    cameraRay.Origin = origin;
    cameraRay.Dir = ray_world;
    return  cameraRay;
}

pSceneOb Scene::pickObject(float ndcX, float ndcY)
{
    Ray cameraRay = castRayFromCamera(ndcX, ndcY);
    float min_t = 0.0;
    float index = 0;

    if(m_pickedObject)
        m_pickedObject->isActive(false);
    m_pickedObject.reset();

    for(uint i = 1; i < m_SceneObjects.size(); i++)
    {

        float t = m_CollisionDetect.checkRaySphereF(cameraRay.Origin,
                                                    cameraRay.Dir.normalized(),
                                                    m_SceneObjects[i]->getPos(),
                                                    0.5);
        if(t > 0.0)
        {
            if(min_t == 0.0)
            {
                m_pickedObject = m_SceneObjects[i];
                m_pickedObject->isActive(true);
                 m_pickedObject->m_IsDirty = true;
//                qDebug()<<"set:"<<i<<t;
                min_t = t;
                index = i;
            }
            if(t < min_t )
            {
                m_pickedObject = m_SceneObjects[i];
                m_pickedObject->isActive(true);
                m_pickedObject->m_IsDirty = true;
//                qDebug()<<"set:"<<i<<t;
                min_t = t;
                index = i;
            }
        }
    }
//    qDebug()<<index<<&m_SceneObjects[index];

    return m_SceneObjects[index];
}

void Scene::updateLinesVBO()
{
    m_lines_vao->bind();
    m_lines_vbo.create();
    m_lines_vbo.bind();

    QVector3D** ptr = m_DynamicsWorld->m_debugLines.data();
    QVector3D* ptr2 = *ptr;
    QVector3D result = *ptr2;

    m_lines_vbo.allocate(*(m_DynamicsWorld->m_debugLines.data()),  m_DynamicsWorld->m_debugLines.size() * sizeof(QVector3D));


//    qDebug()<<result;

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,                 // index
                          3,                 // size of attr
                          GL_FLOAT,          // datatype of each component
                          GL_FALSE,          // normalized
                          sizeof(QVector3D), //byte offset between consecutive generic vertex attributes
                          nullptr);
    m_lines_vao->release();

}

void Scene::debug()
{
//    QVector3D** ptr = m_DynamicsWorld->m_debugLines.data();

//     QVector3D* ptr2 = *ptr;

//     QVector3D result = *ptr2;
//     qDebug()<<result;
}

void Scene::QtOpenGLinitialize()
{
    if(widget())
        widget()->makeCurrent();

    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    m_arcCamera.translate(0.0f, 0.0f, 25.0f);
    m_arcCamera.SetWorldPos(QVector3D(0.0f, 0.0f, 25.0f));
    m_arcCamera.SetPivot(QVector3D(0.0f, 0.0f, 0.0f));
    m_arcCamera.SetPivotToCam(QVector3D(0,0,25));
    m_arcCamera.arcBallStart();

//----build, compline and link shaders
    m_screen_program = new QOpenGLShaderProgram;
    m_screen_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/screen.vert");
    m_screen_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/screen.frag");
    m_screen_program->link();

    m_lighting_program = new QOpenGLShaderProgram();
    m_lighting_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/phongWireframe.vert");
    m_lighting_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/phongWireframe.frag");
    m_lighting_program->link();

    m_flat_program = new QOpenGLShaderProgram();
    m_flat_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/flat.vert");
    m_flat_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/flat.frag");
    m_flat_program->link();

    //----prepare a QuadPlane
//    m_quad_vao = new QOpenGLVertexArrayObject(window());
    m_quad_vao = new QOpenGLVertexArrayObject();
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
//   int SCR_WIDTH = window()->width()*2;
//   int SCR_HEIGHT = window()->height()*2;

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    //-------------texture as color buffer
    glGenTextures(1, &texture);
//    glBindTexture(GL_TEXTURE_2D, texture);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, SCR_WIDTH, SCR_HEIGHT, GL_TRUE);
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

    //Creating a depth and stencil renderbuffer object
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // attach the renderbuffer object to framebuffer
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      qCritical()<<"gBuffer FBO not complete! error enum:"<< (glCheckFramebufferStatus(GL_FRAMEBUFFER));
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

//    m_screen_program->setUniformValue("screenTexture", 0);

    GLuint id = m_screen_program->programId();
    glUniform1i(glGetUniformLocation(id, "screenTexture"), 0);

    m_flat_program->bind();

    m_lines_vao = new QOpenGLVertexArrayObject();
    m_lines_vao->create();
    m_lines_vao->bind();

    m_lines_vbo.create();
    m_lines_vbo.bind();
//    m_lines_vbo.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    m_lines_vbo.allocate(m_Lines.data(), m_Lines.size() * sizeof(Line));


    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(QVector3D),
                          nullptr);
    m_flat_program->release();
}

void Scene::DynamicsInitialize()
{
    if(!m_DynamicsWorld)
        m_DynamicsWorld = new DynamicsWorld();

    m_DynamicsWorld->initialize();
}

void Scene::resize(int width, int height)
{
    if (window())
    {
        SCR_WIDTH = window()->width() * 2;
        SCR_HEIGHT = window()->height() * 2;
        qDebug()<<"window"<<SCR_WIDTH<<SCR_HEIGHT;
    }
    else if(widget())
    {
        SCR_WIDTH = widget()->width() * 2;
        SCR_HEIGHT = widget()->height() * 2;
        qDebug()<<"widget"<<SCR_WIDTH<<SCR_HEIGHT;
    }
    else
    {
        SCR_WIDTH = 720;
        SCR_HEIGHT = 720;
    }


    m_projection_matrix.setToIdentity();
    m_projection_matrix.perspective(40.0f, width / float(height), 0.01f, 1000.0f);

    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, SCR_WIDTH, SCR_HEIGHT, GL_TRUE);

    glBindTexture(GL_TEXTURE_2D, screenTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
//    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
}

void Scene::paint()
{



//    qDebug()<<"paint Start";
    // draw to the framebuffer (off-screen render)
    glViewport(0,0, SCR_WIDTH, SCR_HEIGHT);
    //  glViewport(0,0, SCR_WIDTH, SCR_HEIGHT);
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
            // draw all SceneObjects
                for(uint i = 1; i < m_SceneObjects.size(); i++)
                {

                    int test = m_SceneObjects.size();

                    if(m_pickedObject == m_SceneObjects[i])
                    {
                        uint matID = m_SceneObjects[i]->getMaterialID();
                        m_lighting_program->setUniformValue("wireFrameColor", QVector3D(0,1,0) );
                        m_lighting_program->setUniformValue("overlayColor", QVector4D(0.1,0.5,0.2,1) );
                        m_lighting_program->setUniformValue("mMaterial.ambient", m_Materials[matID]->ambient );
                        m_lighting_program->setUniformValue("mMaterial.diffuse", m_Materials[matID]->diffuse );
                        m_lighting_program->setUniformValue("mMaterial.specular", m_Materials[matID]->specular );
                        m_lighting_program->setUniformValue("mMaterial.shininess", m_Materials[matID]->shininess );
                        m_lighting_program->setUniformValue("ModelMatrix",  m_SceneObjects[i]->getMatrix());
                        m_SceneObjects[i]->draw();
                        m_lighting_program->setUniformValue("wireFrameColor", QVector3D(0,0,0) );
                        m_lighting_program->setUniformValue("overlayColor", QVector4D(0,0,0,0) );
                        continue;
                    }

                    uint matID = m_SceneObjects[i]->getMaterialID();
                    m_lighting_program->setUniformValue("mMaterial.ambient", m_Materials[matID]->ambient );
                    m_lighting_program->setUniformValue("mMaterial.diffuse", m_Materials[matID]->diffuse );
                    m_lighting_program->setUniformValue("mMaterial.specular", m_Materials[matID]->specular );
                    m_lighting_program->setUniformValue("mMaterial.shininess", m_Materials[matID]->shininess );
                    m_lighting_program->setUniformValue("ModelMatrix",  m_SceneObjects[i]->getMatrix());
                    m_SceneObjects[i]->draw();
                }


        //-------------------------Draw Wireframe---------------------------------------------------------------------------
              m_flat_program->bind();
              m_flat_program->setUniformValue("ProjectionMatrix", m_projection_matrix);
              m_flat_program->setUniformValue("ViewMatrix", m_arcCamera.toMatrix());
              m_flat_program->setUniformValue("Color", QVector3D(0.3,0.3,0.3));
              if(m_SceneObjects[0])
              {
                  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
                  m_flat_program->setUniformValue("ModelMatrix",  m_SceneObjects[0]->getMatrix());
                  m_SceneObjects[0]->draw();
              }

       //-------------------------Draw Lines---------------------------------------------------------------------------
//              debug();
              updateLinesVBO();
//              debug();
              {
                m_flat_program->setUniformValue("Color", QVector3D(0.0,0.8,0.0));
                m_lines_vao->bind();
//                 glDrawArrays(GL_LINES, 0, m_Lines.size() * 2);
                glDrawArrays(GL_LINES, 0, dynamicsWorld()->m_debugLines.size() * 4);
                m_lines_vao->release();
              }

//              m_flat_program->release();

    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
    glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glViewport ( 0, 0, SCR_WIDTH, SCR_HEIGHT);
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

void Scene::updateSceneObjects()
{
    for(uint i = 1; i < m_SceneObjects.size(); i++)
    {
        m_SceneObjects[i]->update();
    }
}

void Scene::drawScreenQuad()
{
    glViewport ( 0, 0, SCR_WIDTH, SCR_HEIGHT);
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

void Scene::setDynamicsWorld(DynamicsWorld *_world)
{
    m_DynamicsWorld = _world;
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
//       addSceneObjectFromModel("Icosahedron", 0, QVector3D(0,1,0), QQuaternion(1,0,0,0));
//       addSceneObjectFromModel("bunny", 1, QVector3D(1.5 , 0 ,0 ), QQuaternion(1,0,0,0));
//       addSceneObjectFromModel("nanoSuit", 2, QVector3D(-1.2,0,0), QQuaternion(1,0,0,0));

       // 1 Cell
       auto sceneObject1 = addSceneObjectFromModel("Icosahedron", 0, QVector3D(0,15.0,0), QQuaternion(1,0,0,0));
       auto sceneObject2 = addSceneObjectFromModel("Icosahedron", 2, QVector3D(2,11.0,0), QQuaternion(1,0,0,0));
       auto sceneObject3 = addSceneObjectFromModel("Icosahedron", 1, QVector3D(0,10,0), QQuaternion(1,0,0,0));
       auto sceneObject4 = addSceneObjectFromModel("Icosahedron", 0, QVector3D(-3,0,0), QQuaternion(1,0,0,0));
//       auto sceneObject5 = addSceneObjectFromModel("Icosahedron", 1, QVector3D(2,2,0), QQuaternion(1,0,0,0));
//       auto sceneObject6 = addSceneObjectFromModel("Icosahedron", 2, QVector3D(3,0,0), QQuaternion(1,0,0,0));
       //       auto sceneObject3 = addSceneObjectFromModel("Icosahedron", 2, QVector3D(0.9,2,0.9), QQuaternion(1,0,0,0));

//       // in neighbourhood
//       auto sceneObject4 = addSceneObjectFromModel("Icosahedron", 1, QVector3D(1.9,4.9,1.9), QQuaternion(1,0,0,0));

//       // outside
//       auto sceneObject5 = addSceneObjectFromModel("Icosahedron", 0, QVector3D(2.1,22,0), QQuaternion(1,0,0,0));

       // Cells:
       // 1  1-3-0
       // 2  1-3-0
       // 3  1-3-0
       // 4  1-4-1
       // 5  3-22-0

       makeDynamic(sceneObject1);
       makeDynamic(sceneObject2);
       makeDynamic(sceneObject3);
//       makeDynamic(sceneObject2);
//       makeDynamic(sceneObject3);
//       makeDynamic(sceneObject4);
//       makeDynamic(sceneObject5);
//       makeDynamic(sceneObject6);

//       auto myModel = m_ModelPool["grid1"];

       addLine(QVector3D(0,0,0), QVector3D(0,5,0));

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







