#include "Model.h"

aiScene* Model::getScene()
{
    return scene;
}
void Model::loadFromFileParameters(std::string filePath, unsigned int Flags )
{
    scene = (aiScene *)aiImportFile(filePath.c_str(), Flags);
    if(!scene||scene->mFlags==AI_SCENE_FLAGS_INCOMPLETE||!scene->mRootNode){    ///Deterimine if the scene is null, if there was an error reading, or if the first node is blank
        std::cout<<getName()<<" had error reading: "<<filePath<<":"<<aiGetErrorString()<<std::endl;
        return;
    }
    FileDirectory = filePath.substr(0, filePath.find_last_of('/')); ///Get the file's parent folder
    processNode(scene->mRootNode);                                  ///Begin recursive load loop
}
void Model::processNode(aiNode* node)
{
    for(int i=0;i<node->mNumMeshes;i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        Mesh newMesh;
        newMesh.meshFromAssimp(mesh);
        newMesh.loadTextures(FileDirectory,scene);
        newMesh.calculateTangentsBitangents();
        Meshes.push_back(newMesh);
    }
    for(int i=0;i<node->mNumChildren;i++)
    {
        processNode(node->mChildren[i]);
    }
}
void Model::drawToShader(Shader* shdr)
{
    for(int i=0;i<Meshes.size();i++)
    {
        Meshes[i].drawToShader(shdr);
    }
}

