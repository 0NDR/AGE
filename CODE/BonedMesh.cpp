#include "BonedMesh.h"
void loadFromFile(std::string filepath){;}///<Load the mesh from a file with default parameters
void BonedMesh::meshFromAssimp(aiMesh* m)
{
    AssimpMesh = m;
    aiVector3D Zero3D(0,0,0);
    for(GLuint i =0;i<m->mNumVertices;i++)
    {
            const aiVector3D *Position = &(m->mVertices[i]);
            const aiVector3D *Normal;
            const aiVector3D *TexCoord;
            const aiVector3D *bitangents;
            const aiVector3D *tangents;

            if(m->HasTextureCoords(0)) //Add texture coordinates
            {
                TexCoord =&(m->mTextureCoords[0][i]);
            }
            else
            {
                TexCoord =&Zero3D;
            }

            if(m->HasNormals())  //Add normal coordinates
            {
                Normal = &(m->mNormals[i]);
            }
            else
            {
                Normal = &Zero3D;
            }

            if(m->HasTangentsAndBitangents()) //Add tangents and biTangents.
            {
                tangents = &(m->mTangents[i]);
                bitangents = &(m->mBitangents[i]);
            }
            else
            {
                tangents = &Zero3D;
                bitangents = &Zero3D;
            };
            VertexBoneData newVector; //Create vertex to add to it
            newVector.Position = glm::vec3(Position->x,Position->y,Position->z);          //convert pos
            newVector.TextureCoords = glm::vec2(TexCoord->x,TexCoord->y);                 //conver texture coords
            newVector.Normal = glm::vec3(Normal->x,Normal->y,Normal->z);                  //convert normals
            newVector.Tangent = glm::vec3(tangents->x,tangents->y,tangents->z);           //convert tangentz
            newVector.BiTangent = -glm::vec3(bitangents->x,bitangents->y,bitangents->z);           //convert tangentz
            VertexData.push_back(newVector);
    }
    for (unsigned int f = 0 ; f < m->mNumFaces ; f++)
    {
        const aiFace& Face = m->mFaces[f];
        if(Face.mNumIndices == 3)
        {
            Indices.push_back(Face.mIndices[0]);
            Indices.push_back(Face.mIndices[1]);
            Indices.push_back(Face.mIndices[2]);
        }
    }


}

void BonedMesh::drawToShader(Shader* shdr)
{
    GLuint ShaderProgram = shdr->ShaderProgram;
    //for(int i=0;i<VertexData.size();i++)
        //std::cout<<VertexData[i].Weights[0]<<" "<<VertexData[i].boneIndicies[0]<<std::endl;

    for(GLuint i = 0;i<Textures.size();i++)
    {
        Textures[i]->Activate();
        Textures[i]->bindTexture();
        Textures[i]->AttachToShader(shdr);
    }
    GLint pA = glGetAttribLocation(ShaderProgram, "position");
    GLint nA = glGetAttribLocation(ShaderProgram, "normal");
    GLint teA = glGetAttribLocation(ShaderProgram, "texcoord");
    GLint taA = glGetAttribLocation(ShaderProgram, "tangent");
    GLint baA = glGetAttribLocation(ShaderProgram, "bitangent");
    GLint biA = glGetAttribLocation(ShaderProgram, "BoneID");
    GLint bwA = glGetAttribLocation(ShaderProgram, "BoneWeight");
    glEnableVertexAttribArray(pA);
    glEnableVertexAttribArray(nA);
    glEnableVertexAttribArray(teA);
    glEnableVertexAttribArray(taA);
    glEnableVertexAttribArray(baA);
    glEnableVertexAttribArray(biA);
    glEnableVertexAttribArray(bwA);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
    glVertexAttribPointer(pA, 3, GL_FLOAT, GL_FALSE,  sizeof(VertexBoneData), 0);
    glVertexAttribPointer(nA, 3, GL_FLOAT, GL_FALSE,  sizeof(VertexBoneData), (const GLvoid*)offsetof(VertexBoneData,Normal));
    glVertexAttribPointer(teA, 2, GL_FLOAT, GL_FALSE,  sizeof(VertexBoneData), (const GLvoid*)offsetof(VertexBoneData,TextureCoords));
    glVertexAttribPointer(taA, 3, GL_FLOAT, GL_FALSE,  sizeof(VertexBoneData), (const GLvoid*)offsetof(VertexBoneData,Tangent));
    glVertexAttribPointer(baA, 3, GL_FLOAT, GL_FALSE,  sizeof(VertexBoneData), (const GLvoid*)offsetof(VertexBoneData,BiTangent));
    glVertexAttribIPointer(biA, BonesPerVertex, GL_INT,  sizeof(VertexBoneData), (const GLvoid*)offsetof(VertexBoneData,boneIndicies));
    glVertexAttribPointer(bwA, BonesPerVertex, GL_FLOAT, GL_FALSE,  sizeof(VertexBoneData), (const GLvoid*)(offsetof(VertexBoneData,Weights)));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);

    glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(pA);
    glDisableVertexAttribArray(nA);
    glDisableVertexAttribArray(teA);
    glDisableVertexAttribArray(taA);
    glDisableVertexAttribArray(baA);
    glDisableVertexAttribArray(biA);
    glDisableVertexAttribArray(bwA);
}

void BonedMesh::loadBones(std::map<std::string,int>& boneMap,std::vector<BoneInfo> &infos,int& numBones)
{
    for(int bIndex = 0;bIndex<AssimpMesh->mNumBones;bIndex++)
    {
        aiBone* bone = AssimpMesh->mBones[bIndex];
        std::string bName =bone->mName.C_Str();
        int IndexInMap = 0;
        if(boneMap.find(bName)==boneMap.end())
        {
            IndexInMap = numBones;
            numBones++;
            BoneInfo inf;
            infos.push_back(inf);
            infos[IndexInMap].boneName = bName;
            infos[IndexInMap].offset = glm::transpose(glm::make_mat4(&bone->mOffsetMatrix.a1));
            boneMap[bName]= IndexInMap;
        }
        else
        {
            IndexInMap = boneMap[bName];
        }

        for(int wIndex=0;wIndex<bone->mNumWeights;wIndex++)
        {
            aiVertexWeight weight = bone->mWeights[wIndex];
            int vertIndex = weight.mVertexId;
            float boneWeight = weight.mWeight;
            for(int vbIndex=0;vbIndex<BonesPerVertex;vbIndex++)
            {
                if(VertexData[vertIndex].Weights[vbIndex]==0)
                {
                    VertexData[vertIndex].Weights[vbIndex]=boneWeight;
                    VertexData[vertIndex].boneIndicies[vbIndex]=IndexInMap;
                    break;
                }
            }
        }
    }
    glGenBuffers(1,&VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBoneData)*VertexData.size(),&VertexData[0], GL_STATIC_DRAW);

    glGenBuffers(1,&IndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*Indices.size(), &Indices[0], GL_STATIC_DRAW);
}
