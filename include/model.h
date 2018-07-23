#ifndef MODEL_H
#define MODEL_H

#include "shape.h"
#include <iostream>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

class Model
{
public:
    Model();
    void loadModel(std::string _path);
    void processNode(aiNode *node, const aiScene *scene);
    void draw();
    void bind();
    Shape processMesh(aiMesh *mesh, const aiScene *scene);

private:
    std::string directory;
    // could be to pointer of Shapp (that's what's discussed to save resources)
    std::vector<Shape> meshes;

};



#endif // MODEL_H
