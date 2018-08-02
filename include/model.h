#ifndef MODEL_H
#define MODEL_H


#include <iostream>
#include <vector>

#include <QtGui/QOpenGLShaderProgram>

#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "shape.h"
#include "utils.h"





// forward declare Scene, to enable passing *scene and *shader to meshes.
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
    ShapePtr processMesh(aiMesh *mesh, const aiScene *scene);

    void inline foo(){qDebug()<<"hello model";};

//private:
    std::string directory;
    // could be to pointer of Shapp (that's what's discussed to save resources)
//    std::vector<Shape *> meshes;
    std::vector<ShapePtr> meshes;
    Scene *pScene;
    QOpenGLShaderProgram *pShader;

};

#endif // MODEL_H