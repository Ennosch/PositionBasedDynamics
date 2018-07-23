#include "include/model.h"

#include <QDebug>

#define myqDebug() qDebug() << fixed << qSetRealNumberPrecision(3)
Model::Model()
{

}


void Model::loadModel(std::string _path)
{
    Assimp::Importer importer;
//    std::cout<<_path<<std::endl;


    const aiScene* scene = importer.ReadFile(_path,
                                             aiProcess_Triangulate |
                                             aiProcess_FlipUVs |
                                             aiProcess_CalcTangentSpace |
                                             aiProcess_JoinIdenticalVertices);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
            {
                std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
                return;
            }
    directory = _path.substr(0, _path.find_last_of('/'));
//    std::cout<<directory<<std::endl;
    processNode(scene->mRootNode, scene);

    auto index = scene->mMeshes[0]->mFaces[0].mIndices[0];

//    for(int i = 0; i < 11; i++)
//    {
//        auto x = scene->mMeshes[0]->mFaces[i].mIndices[0];
//        auto y = scene->mMeshes[0]->mFaces[i].mIndices[1];
//        auto z = scene->mMeshes[0]->mFaces[i].mIndices[2];

//        auto v11 = scene->mMeshes[0]->mVertices[x].x;
//        auto v12 = scene->mMeshes[0]->mVertices[x].y;
//        auto v13 = scene->mMeshes[0]->mVertices[x].z;

//        auto v21 = scene->mMeshes[0]->mVertices[y].x;
//        auto v22 = scene->mMeshes[0]->mVertices[y].y;
//        auto v23 = scene->mMeshes[0]->mVertices[y].z;

//        auto v31 = scene->mMeshes[0]->mVertices[z].x;
//        auto v32 = scene->mMeshes[0]->mVertices[z].y;
//        auto v33 = scene->mMeshes[0]->mVertices[z].z;
//        myqDebug()<<"f"<<x+1<<y+1<<z+1<<" v"<<v11<<v12<<v13<<"|"<<v21<<v22<<v23<<"|"<<v31<<v32<<v33;
//    }
}

void Model::processNode(aiNode *node, const aiScene *scene)
{

    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene.
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Shape Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    // data to fill
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    // Walk through each of the mesh's vertices
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
//        Vertex vertex;
//        QVector3D vector;
//        vector.setX(mesh->mVertices[i].x);
//        vector.setY(mesh->mVertices[i].y);
//        vector.setZ(mesh->mVertices[i].z);
//        vertex.Position = vector;
//        vector.setX(mesh->mNormals[i].x);
//        vector.setY(mesh->mNormals[i].y);
//        vector.setZ(mesh->mNormals[i].z);
//        vertex.Normal = vector;


//        qDebug()<<vertex.Position<<vertex.Normal;

        Vertex vertex;
        QVector3D vector = QVector3D(0,0,0);
        vector.setX(1);

        vertex.Position = vector;
        vertex.Normal = vector;
//        qDebug()<<vertex.Position<<vertex.Normal;

//        qDebug()<<mesh->mVertices[i].x<<mesh->mVertices[i].y;


//      vertices.push_back(vertex);
    }
    qDebug()<<"-----------";
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for(unsigned int j = 0; j < face.mNumIndices; j++)
        {
          indices.push_back(face.mIndices[j]);
        }
    }
    return Shape(vertices, indices);
}

void Model::draw()
{
    for(unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].draw();
}

void Model::bind()
{
    for(unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].bind();
}

