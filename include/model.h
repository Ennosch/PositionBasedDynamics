#ifndef MODEL_H
#define MODEL_H

#include "shape.h"
#include <iostream>
#include <vector>

#include <QtGui/QOpenGLShaderProgram>
//#include "Scene.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>



class Scene;


class Model
{
public:
    Model();
    Model(Scene *_scene, QOpenGLShaderProgram *_shaderProgram);
    void loadModel(std::string _path);
    void processNode(aiNode *node, const aiScene *scene);
    void draw();
    void bind();
    Shape processMesh(aiMesh *mesh, const aiScene *scene);

        void inline foo(){qDebug()<<"hello model";};

//private:
    std::string directory;
    // could be to pointer of Shapp (that's what's discussed to save resources)
    std::vector<Shape> meshes;
    Scene *pScene;
    QOpenGLShaderProgram *pShader;
};



#endif // MODEL_H
