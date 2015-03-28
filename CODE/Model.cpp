#include "Model.h"
#include "BonedMesh.h"
#include <Assimp/Importer.hpp>
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
    scene = (aiScene*)aiApplyPostProcessing(scene,aiProcess_CalcTangentSpace);
    FileDirectory = filePath.substr(0, filePath.find_last_of('/')); ///Get the file's parent folder
    processNode(scene->mRootNode);                                  ///Begin recursive load loop

}
void Model::loadFromAssimp(aiScene* nscene)
{
    scene = nscene;
    processNode(scene->mRootNode);                                  ///Begin recursive load loop

}
void Model::processNode(aiNode* node)
{
    for(int i=0;i<node->mNumMeshes;i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        std::string name = (std::string)mesh->mName.C_Str();
        if(name=="")
        {
            std::stringstream s;
            s<<"MESH"<<Meshes.size()<<"OBJ";
            name=s.str();
        }
        Mesh* newMesh;
        newMesh = new Mesh(this,name);
        newMesh->meshFromAssimp(mesh);
        newMesh->loadTextures(FileDirectory,scene);
        //newMesh->calculateTangentsBitangents();
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
        glUniform1i(glGetUniformLocation(shdr->ShaderProgram,"HasBones"),(int)0);
        Meshes[i]->drawToShader(shdr);
    }
}

void Model::saveToFile(std::string filepath)
{
    std::string directory = filepath.substr(0,filepath.find_last_of('/')+1);
    std::string nameBase = filepath.substr(filepath.find_last_of('/')+1,filepath.find_last_of('.')-filepath.find_last_of('/')-1);
    std::string pathWithoutExtension = directory+nameBase;
    int NumIndicies = 0;
    std::ofstream outOBJ(filepath.c_str());
    std::ofstream outMTL((pathWithoutExtension+".mtl").c_str());
    outOBJ<<"mtllib "<<nameBase<<".mtl"<<std::endl;
    for(int i=0;i<Meshes.size();i++)
    {
        Mesh* m = Meshes[i];
        outOBJ<<"o "<<m->getName()<<std::endl;
        for(int i=0;i<m->VertexData.size();i++)
        {
            outOBJ<<"v "<<m->VertexData[i].Position.x<<" "<<m->VertexData[i].Position.y<<" "<<m->VertexData[i].Position.z<<std::endl;
            outOBJ<<"vt "<<m->VertexData[i].TextureCoords.x<<" "<<m->VertexData[i].TextureCoords.y<<" "<<std::endl;
            outOBJ<<"vn "<<m->VertexData[i].Normal.x<<" "<<m->VertexData[i].Normal.y<<" "<<m->VertexData[i].Normal.z<<std::endl;
        }
        outOBJ<<"usemtl "<<m->getName()<<"mat"<<std::endl;
        for(int i=0;i<m->Indices.size()/3;i++)
        {
            outOBJ<<"f "<<NumIndicies+m->Indices[i*3]+1<<"/"<<NumIndicies+m->Indices[i*3]+1<<"/"<<NumIndicies+m->Indices[i*3]+1<<" "
                        <<NumIndicies+m->Indices[i*3+1]+1<<"/"<<NumIndicies+m->Indices[i*3+1]+1<<"/"<<NumIndicies+m->Indices[i*3+1]+1<<" "
                        <<NumIndicies+m->Indices[i*3+2]+1<<"/"<<NumIndicies+m->Indices[i*3+2]+1<<"/"<<NumIndicies+m->Indices[i*3+2]+1<<std::endl;
        }
        NumIndicies+=m->Indices.size();
        outMTL<<"newmtl "<<m->getName()<<"mat"<<std::endl;
        outMTL<<"Ns 10.000"<<std::endl;
        outMTL<<"Ka 1.0000 1.00000 1.00000"<<std::endl;
        outMTL<<"Kd 1.0000 1.00000 1.00000"<<std::endl;
        outMTL<<"Ks 0.0000 0.00000 0.00000"<<std::endl;
        outMTL<<"illum 2"<<std::endl;
        for(int i=0;i<m->Textures.size();i++)
        {
            if(MTLTextureTypeNames[m->Textures[i]->getTextureType()]!="X")
            {
                outMTL<<MTLTextureTypeNames[m->Textures[i]->getTextureType()]<<" "<<m->Textures[i]->getName()<<std::endl;
                m->Textures[i]->saveToFile(directory+m->Textures[i]->getName());
            }

        }
    }
    outOBJ.close();
    outMTL.close();
}
