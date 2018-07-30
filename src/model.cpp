#include "include/model.h"

#include <QDebug>

#define myqDebug() qDebug() << fixed << qSetRealNumberPrecision(3)
Model::Model()
{

}

Model::Model(Scene *_scene, QOpenGLShaderProgram *_shaderProgram)
            : pScene(_scene), pShader(_shaderProgram)
{
    qDebug()<<"construct Model with Scene and Shader";
}


void Model::loadModel(std::string _path)
{
    Assimp::Importer importer;
//    std::cout<<_path<<std::endl;

    const aiScene* scene = importer.ReadFile(_path,
                                             aiProcess_Triangulate |
                                             aiProcess_FlipUVs |
                                             aiProcess_CalcTangentSpace |
                                             aiProcess_JoinIdenticalVertices |
                                             aiProcess_FlipWindingOrder |
                                             aiProcess_GenNormals |

                                             aiProcess_FixInfacingNormals);

//    Assimp::Exporter exporter;
////    exporter.Export();
//    auto count = exporter.GetExportFormatCount();
//    auto format1 = exporter.GetExportFormatDescription(0);
//    qDebug()<<format1->description;

////    auto caught = exporter.Export(scene, "Wavefront OBJ format", "resources/objects/rock/export.obj");
//    auto caught = exporter.Export(scene,  "obj", "resources/objects/rock/AssimpExport.obj", aiProcess_FlipWindingOrder);
//    qDebug()<<"NO EXPORT: "<<exporter.GetErrorString();

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
            {
                std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
                return;
            }
    directory = _path.substr(0, _path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene.
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

        ShapePtr ptrShape = processMesh(mesh, scene);

        if(ptrShape->vertices.data() != nullptr)
        {
            // (note, pushing_back for objects on the stack calls copy ctor)
            meshes.push_back(ptrShape);
        }
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

ShapePtr Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    // data to fill
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // Walk through each of the mesh's vertices
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        QVector3D vector;
        vector.setX(mesh->mVertices[i].x);
        vector.setY(mesh->mVertices[i].y);
        vector.setZ(mesh->mVertices[i].z);
        vertex.Position = vector;
        vector.setX(mesh->mNormals[i].x);
        vector.setY(mesh->mNormals[i].y);
        vector.setZ(mesh->mNormals[i].z);
        vertex.Normal = vector;

        vertices.push_back(vertex);
    }

    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for(unsigned int j = 0; j < face.mNumIndices; j++)
        {
          indices.push_back(face.mIndices[j]);
        }
    }

//    Shape *ptrNewShape = new Shape(vertices, indices, pScene, pShader);
//    return ptrNewShape;
//    ShapePtr ptrNewShape = std::make_shared<Shape>(vertices, indices, pScene, pShader);
     ShapePtr ptrNewShape = std::make_shared<Shape>(vertices, indices);
    return ptrNewShape;
}

void Model::draw()
{
    for(unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i]->draw();
    }
}

void Model::bind()
{
    for(unsigned int i = 0; i < meshes.size(); i++)
        meshes[i]->bind();
}

