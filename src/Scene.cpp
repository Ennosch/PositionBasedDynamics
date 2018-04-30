////////////////////////////////////////////////////////////////////////////////
/// @file Scene.cpp
/// @author Ramon Blanquer
/// @version 0.0.1
////////////////////////////////////////////////////////////////////////////////

// Project
#include "Scene.h"
#include <array>
#include <iostream>

#include <QFile>
#include <QDebug>
#include <QString>

/*
#include &lt;QtGui/QOpenGLContext&gt;
#include &lt;QtGui/QOpenGLBuffer&gt;
#include &lt;QtGui/QOpenGLVertexArrayObject&gt;
*/
Scene::Scene(Window *_window) : AbstractScene(_window)
{
}

Scene::~Scene()
{
}

void Scene::initialize()
{
  AbstractScene::initialize();
  OpenGLinitialize();
  //QtOpenGLinitialize();
}

void Scene::QtOpenGLinitialize()
{
    qDebug()<<"init 1";
    m_program = new QOpenGLShaderProgram();
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":shader/geom.vert");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/geom.frag");
    m_program->link();

    GLfloat verts_pos[] ={
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };
    //QOpenGLBuffer m_vvbo(QOpenGLBuffer::VertexBuffer);
    m_vao.create();
    m_vao.bind();
    m_vvbo.create();
    m_vvbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vvbo.bind();
    m_vvbo.allocate(verts_pos, sizeof(verts_pos));
    //std::cout<<"log ?: "<< sizeof(verts_pos);

    m_program->enableAttributeArray(0);
    m_program->setAttributeBuffer(0, GL_FLOAT, 0, 3);

    m_vvbo.release();
    m_vao.release();
    qDebug()<<"init 2";
}

void Scene::OpenGLinitialize()
{
    GLfloat vertices[] ={
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };
    //unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);


    QString Filename =  ":/shader/simple.vert";
    QFile mFile(Filename);
    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug()<< "could not open file for writing";
        return;
    }
    QTextStream in (&mFile);
    QString mText = in.readAll();
    qDebug() << mText;
    mFile.close();

    std::string str = "#version 330 core\nlayout (location = 0) in vec3 aPos;\n\nvoid main()\n{\n    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n}\n";
    const char* vertexShaderSource = str.c_str();

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }


    QString Filename2 =  ":/shader/simple.frag";
    QFile mFile2(Filename2);
    if(!mFile2.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug()<< "could not open file for writing";
        return;
    }

    QTextStream in2 (&mFile2);
    QString mText2 = in2.readAll();
    qDebug() << mText2;
    mFile2.close();


    std::string str2 = "#version 330 core\nout vec4 FragColor;\n\nvoid main()\n{\n    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n}\0";
    const char* fragmentShaderSource = str2.c_str();

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);


    int  success2;
    char infoLog2[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success2);
    if(!success2)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog2);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog2 << std::endl;
    }

    //unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "LINKING FAILED\n" << infoLog << std::endl;
    }


    //unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    qDebug()<<"endInit ";
}

void Scene::paint()
{
  glViewport(0, 0, window()->width(), window()->height());
  glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);


  /*
  // render using the Qt classes
  m_vao.bind();
  glDrawArrays(GL_TRIANGLES, 0, 3);
  m_vao.release();
  //std::cout<<"spamm"<<std::endl;


  // how to purely render a VBO

  m_vvbo.bind();
  m_program->bind();
  glDrawArrays(GL_TRIANGLES, 0, 3);
  m_vvbo.release();
  m_program->release();
  */
  glUseProgram(shaderProgram);
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 3);

}
