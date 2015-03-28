#include "BonedModel.h"
#include "BonedMesh.h"
#include <Assimp/Importer.hpp>
glm::mat4 mat4FromAssimp(aiMatrix4x4 m)
{
    return glm::transpose(glm::make_mat4(&m.a1));
}
glm::quat quatFromAssimp(aiQuaternion q)
{
    return glm::quat(q.w,q.x,q.y,q.z);
}

void BonedModel::loadFromFileParameters(std::string filePath, unsigned int Flags )
{
    scene = (aiScene *)aiImportFile(filePath.c_str(), Flags);
    if(!scene||scene->mFlags==AI_SCENE_FLAGS_INCOMPLETE||!scene->mRootNode){    ///Deterimine if the scene is null, if there was an error reading, or if the first node is blank
        std::cout<<getName()<<" had error reading: "<<filePath<<":"<<aiGetErrorString()<<std::endl;
        return;
    }
    scene = (aiScene*)aiApplyPostProcessing(scene,aiProcess_CalcTangentSpace);
    aiMatrix4x4 trans = scene->mRootNode->mTransformation;
    GlobalTransform =mat4FromAssimp(trans);
    FileDirectory = filePath.substr(0, filePath.find_last_of('/')); ///Get the file's parent folder
    processNode(scene->mRootNode);                                  ///Begin recursive load loop

}
void BonedModel::loadFromAssimp(aiScene* nscene)
{
    scene = nscene;
    processNode(scene->mRootNode);                                  ///Begin recursive load loop

}
void BonedModel::processNode(aiNode* node)
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
        BonedMesh* newMesh = new BonedMesh(this,name);
        newMesh->meshFromAssimp(mesh);
        newMesh->loadBones(boneMap,boneInfos,numBones);
        newMesh->loadTextures(FileDirectory,scene);
        //newMesh->calculateTangentsBitangents();
        Meshes.push_back(newMesh);
    }
    for(int i=0;i<node->mNumChildren;i++)
    {
        processNode(node->mChildren[i]);
    }
}
void BonedModel::drawToShader(Shader* shdr)
{

    for(int i=0;i<boneTransforms.size();i++)
    {

        char name[128];
        sprintf(name,"Bones[%d]",i);
        glUniformMatrix4fv(glGetUniformLocation(shdr->ShaderProgram,name),1,GL_FALSE,glm::value_ptr(boneTransforms[i]));
    }
    for(int i=0;i<Meshes.size();i++)
    {
        glUniform1i(glGetUniformLocation(shdr->ShaderProgram,"HasBones"),1);
        ((BonedModel*)Meshes[i])->drawToShader(shdr);
    }
}

void BonedModel::setTime(double time,std::string AnimationName)
{
    aiAnimation *anim=NULL;
    for(int i=0;i<scene->mNumAnimations;i++)
    {
        if(scene->mAnimations[i]->mName.C_Str() == AnimationName)
        {
            anim = scene->mAnimations[i];
            break;
        }
    }
    if(anim==NULL){anim=scene->mAnimations[0];}
    float ticksPassed = time*(anim->mTicksPerSecond!=0?anim->mTicksPerSecond:25.f);
    float animTime = fmod(ticksPassed,(float)anim->mDuration);
    glm::mat4 identity(1.0f);
    readHeirarchy(animTime, scene->mRootNode,identity,anim);

    boneTransforms.resize(numBones);
    for(int i=0;i<numBones;i++)
    {
        boneTransforms[i] = boneInfos[i].finalt;
    }
}
glm::mat4 BonedModel::getTransform(std::string name)
{
    return boneTransforms[boneMap[name]];
}
void BonedModel::readHeirarchy(float t, aiNode* node, glm::mat4 &TransformMat, aiAnimation *anim)
{
    std::string nodeName = node->mName.C_Str();
    glm::mat4 NodeTrans = mat4FromAssimp(node->mTransformation);
    aiNodeAnim *nodeAnimation = NULL;
    for(int i=0;i<anim->mNumChannels;i++)
    {
        if(anim->mChannels[i]->mNodeName.C_Str() == nodeName)
        {
            nodeAnimation = anim->mChannels[i];
            break;
        }
    }
    if(nodeAnimation)
    {
        glm::mat4 scaltrans, postrans, rottrans;
        glm::vec3 Scale    = InterpolatedScale(t,nodeAnimation);
        glm::vec3 Position = InterpolatedPosition(t,nodeAnimation);
        glm::quat Rotation = InterpolatedRotation(t,nodeAnimation);
        scaltrans = glm::scale(scaltrans,Scale);
        postrans = glm::translate(postrans,Position);
        rottrans = glm::mat4_cast(Rotation);
        NodeTrans = postrans*rottrans*scaltrans;

    }
    glm::mat4 globalTransform = TransformMat*NodeTrans;
    if(boneMap.find(nodeName) != boneMap.end())
    {
        int BoneIndex = boneMap[nodeName];
        boneInfos[BoneIndex].finalt = globalTransform*boneInfos[BoneIndex].offset;
    }
    for (int i = 0 ; i < node->mNumChildren ; i++) {
        readHeirarchy(t, node->mChildren[i], globalTransform,anim);
    }

}

glm::quat BonedModel::InterpolatedRotation(float t, aiNodeAnim* nodeAnimation){
    // we need at least two values to interpolate...
    if (nodeAnimation->mNumRotationKeys == 1) {
        return  quatFromAssimp(nodeAnimation->mRotationKeys[0].mValue);
    }
    int RotationIndex = 0;
    for(int i=0;i<nodeAnimation->mNumRotationKeys-1;i++)
    {
        if(t<nodeAnimation->mRotationKeys[i + 1].mTime)
        {
            RotationIndex = i;
            break;
        }

    }
    int NextRotationIndex = (RotationIndex + 1);
    int tFirst = nodeAnimation->mRotationKeys[RotationIndex].mTime;
    int tNext = nodeAnimation->mRotationKeys[NextRotationIndex].mTime;
    float DeltaTime = tNext-tFirst;
    float Factor = (t - (float)tFirst)/DeltaTime;
    glm::quat StartRotationQ = glm::normalize(quatFromAssimp(nodeAnimation->mRotationKeys[RotationIndex].mValue));
    glm::quat EndRotationQ = glm::normalize(quatFromAssimp(nodeAnimation->mRotationKeys[NextRotationIndex].mValue));
    return glm::slerp(StartRotationQ, EndRotationQ, Factor);
}
glm::vec3 BonedModel::InterpolatedPosition(float t, aiNodeAnim* nodeAnimation){
    if (nodeAnimation->mNumPositionKeys == 1) {
        return glm::make_vec3(&nodeAnimation->mPositionKeys[0].mValue[0]);
    }

    int PositionIndex = 0;
    for (int i = 0;i<nodeAnimation->mNumPositionKeys - 1;i++) {
        if (t <nodeAnimation->mPositionKeys[i + 1].mTime) {
            PositionIndex =i;
            break;
        }
    }
    int NextPositionIndex = (PositionIndex + 1);
    float DeltaTime = (float)(nodeAnimation->mPositionKeys[NextPositionIndex].mTime - nodeAnimation->mPositionKeys[PositionIndex].mTime);
    float Factor = (t - (float)nodeAnimation->mPositionKeys[PositionIndex].mTime) / DeltaTime;
    const glm::vec3& Start = glm::make_vec3(&nodeAnimation->mPositionKeys[PositionIndex].mValue[0]);
    const glm::vec3& End = glm::make_vec3(&nodeAnimation->mPositionKeys[NextPositionIndex].mValue[0]);
    glm::vec3 Delta = End - Start;
    return Start + Factor * Delta;
}
glm::vec3 BonedModel::InterpolatedScale(float t, aiNodeAnim* nodeAnimation){
    if (nodeAnimation->mNumScalingKeys == 1) {
        return glm::make_vec3(&nodeAnimation->mScalingKeys[0].mValue[0]);
    }

    int ScaleIndex = 0;
    for (int i = 0 ; i < nodeAnimation->mNumScalingKeys - 1 ; i++) {
        if (t < (float)nodeAnimation->mScalingKeys[i + 1].mTime) {
            ScaleIndex =  i;
            break;
        }
    }
    int NextScaleIndex = (ScaleIndex + 1);
    float DeltaTime = (float)(nodeAnimation->mScalingKeys[NextScaleIndex].mTime - nodeAnimation->mPositionKeys[ScaleIndex].mTime);
    float Factor = (t - (float)nodeAnimation->mScalingKeys[ScaleIndex].mTime) / DeltaTime;
    const glm::vec3& Start = glm::make_vec3(&nodeAnimation->mScalingKeys[ScaleIndex].mValue[0]);
    const glm::vec3& End = glm::make_vec3(&nodeAnimation->mScalingKeys[NextScaleIndex].mValue[0]);
    glm::vec3 Delta = End - Start;
    return Start + Factor * Delta;
}
void BonedModel::saveToFile(std::string filepath)
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
