#include "Mesh.h"
#include "ResourceFactory.h"
#include <fstream>
#include <iostream>
 std::string MTLTextureTypeNames[0xC]  = {"X","map_Kd","map_Ks","map_Ka",
                         "X","map_Bump","X","X",
                         "X","disp","Texture_Lightmap","refl",
                         };
aiMesh* Mesh::getMesh()
{
    return AssimpMesh;
}
void Mesh::meshFromVector(std::vector<Vertex> Verts)
{
    std::vector<unsigned int> temp;
    for(int i=0;i<Verts.size();i++)
    {
        temp.push_back(i);
    }
    meshFromVector(Verts,temp);
}

void Mesh::meshFromVector(std::vector<Vertex> Verts, std::vector<unsigned int> inds)
{
    VertexData = Verts;
    Indices = inds;
    glGenBuffers(1,&VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*VertexData.size(),&VertexData[0], GL_STATIC_DRAW);

    glGenBuffers(1,&IndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*Indices.size(), &Indices[0], GL_STATIC_DRAW);
}
void Mesh::meshFromAssimp(aiMesh* m)
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
            Vertex newVector; //Create vertex to add to it

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
    glGenBuffers(1,&VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*VertexData.size(),&VertexData[0], GL_STATIC_DRAW);

    glGenBuffers(1,&IndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*Indices.size(), &Indices[0], GL_STATIC_DRAW);

}

void Mesh::loadTextures(std::string pathToDirectory, aiScene *scene)
{
    if(AssimpMesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[AssimpMesh->mMaterialIndex];
        // We assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
        // Same applies to other texture as the following list summarizes:
        // Diffuse: texture_diffuseN
        // Specular: texture_specularN
        // Normal: texture_normalN
        for(int i = 0x1;i<0xC;i+=0x1)
            loadTexturesFromMaterial(material,(aiTextureType)i,pathToDirectory);
    }
}
void Mesh::loadTexturesFromMaterial(aiMaterial* mat, aiTextureType type, std::string dir)
{
    for(GLuint i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        glTexture* texture = new glTexture(this,str.C_Str());
        texture->loadFromFile(dir+"/"+std::string(str.C_Str()));
        texture->setTextureType(type);
        std::string TextureName;
        TextureName = glTexture::TypeUniforms[type];
        std::stringstream s;
        s<<TextureName<<i;
        TextureName = s.str();
        texture->setUniformLocation(Textures.size(),(char*)TextureName.c_str());
        texture->setTarget(GL_TEXTURE_2D);
        texture->setTextureProperty(GL_TEXTURE_WRAP_S,GL_REPEAT);
        texture->setTextureProperty(GL_TEXTURE_WRAP_T,GL_REPEAT);
        texture->setTextureProperty(GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
        texture->setTextureProperty(GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        texture->loadTexture();
        Textures.push_back(texture);
    }
}
void Mesh::drawToShader(Shader* shdr)
{
    GLuint ShaderProgram = shdr->ShaderProgram;

    GLint pA = glGetAttribLocation(ShaderProgram, "position");
    GLint nA = glGetAttribLocation(ShaderProgram, "normal");
    GLint teA = glGetAttribLocation(ShaderProgram, "texcoord");
    GLint taA = glGetAttribLocation(ShaderProgram, "tangent");
    GLint baA = glGetAttribLocation(ShaderProgram, "bitangent");

    for(GLuint i = 0;i<Textures.size();i++)
    {
        Textures[i]->Activate();
        Textures[i]->bindTexture();
        Textures[i]->AttachToShader(shdr);
    }

    glEnableVertexAttribArray(pA);
    glEnableVertexAttribArray(nA);
    glEnableVertexAttribArray(teA);
    glEnableVertexAttribArray(taA);
    glEnableVertexAttribArray(baA);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
    glVertexAttribPointer(pA, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex), 0);
    glVertexAttribPointer(nA, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (const GLvoid*)offsetof(Vertex,Normal));
    glVertexAttribPointer(teA, 2, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (const GLvoid*)offsetof(Vertex,TextureCoords));
    glVertexAttribPointer(taA, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (const GLvoid*)offsetof(Vertex,Tangent));
    glVertexAttribPointer(baA, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (const GLvoid*)offsetof(Vertex,BiTangent));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);

    glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ARRAY_BUFFER,0);

    glDisableVertexAttribArray(pA);
    glDisableVertexAttribArray(nA);
    glDisableVertexAttribArray(teA);
    glDisableVertexAttribArray(taA);
    glDisableVertexAttribArray(baA);

}

void Mesh::calculateTangentsBitangents()
{
    for (unsigned int i = 0 ; i < Indices.size() ; i += 3) {
        Vertex& v0 = VertexData[Indices[i]];
        Vertex& v1 = VertexData[Indices[i+1]];
        Vertex& v2 = VertexData[Indices[i+2]];

        glm::vec3 Edge1 = v1.Position - v0.Position;
        glm::vec3 Edge2 = v2.Position - v0.Position;

        float DeltaU1 = v1.TextureCoords.x - v0.TextureCoords.x;
        float DeltaV1 = v1.TextureCoords.y - v0.TextureCoords.y;
        float DeltaU2 = v2.TextureCoords.x - v0.TextureCoords.x;
        float DeltaV2 = v2.TextureCoords.y - v0.TextureCoords.y;

        float f = 1.0f / (DeltaU1 * DeltaV2 - DeltaU2 * DeltaV1);

        glm::vec3 Tangent;

        Tangent.x = f * (DeltaV2 * Edge1.x - DeltaV1 * Edge2.x);
        Tangent.y = f * (DeltaV2 * Edge1.y - DeltaV1 * Edge2.y);
        Tangent.z = f * (DeltaV2 * Edge1.z - DeltaV1 * Edge2.z);

        v0.Tangent += Tangent;
        v1.Tangent += Tangent;
        v2.Tangent += Tangent;

        float x1 = v1.Position.x - v0.Position.x;
        float x2 = v2.Position.x - v0.Position.x;
        float y1 = v1.Position.y - v0.Position.y;
        float y2 = v2.Position.y - v0.Position.y;
        float z1 = v1.Position.z - v0.Position.z;
        float z2 = v2.Position.z - v0.Position.z;

        float s1 = v1.TextureCoords.x - v0.TextureCoords.x;
        float s2 = v2.TextureCoords.x - v0.TextureCoords.x;
        float t1 = v1.TextureCoords.y - v0.TextureCoords.y;
        float t2 = v2.TextureCoords.y - v0.TextureCoords.y;

        float r = 1.0F / (s1 * t2 - s2 * t1);
        glm::vec3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
                (t2 * z1 - t1 * z2) * r);
        glm::vec3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
                (s1 * z2 - s2 * z1) * r);

        v0.tan1 += sdir;
        v1.tan1 += sdir;
        v2.tan1 += sdir;

        v0.tan2 += tdir;
        v1.tan2 += tdir;
        v2.tan2 += tdir;
    }
    for (unsigned int i = 0 ; i < Indices.size() ; i ++)
    {
        Vertex& v = VertexData[Indices[i]];
        glm::vec3 t = v.tan1;
        glm::vec3 n = v.Normal;
        v.Tangent = glm::normalize(t-n*glm::dot(n,t));
        v.BiTangent = (((glm::dot(glm::cross(n,t),v.tan2))<0.0)?-1.0f:1.0f)*glm::cross(n,t);
    }
}

void Mesh::saveToFile(std::string filepath)
{
    std::string directory = filepath.substr(0,filepath.find_last_of('/')+1);
    std::string nameBase = filepath.substr(filepath.find_last_of('/')+1,filepath.find_last_of('.')-filepath.find_last_of('/')-1);
    std::string pathWithoutExtension = directory+nameBase;
    std::ofstream outOBJ(filepath.c_str());
    std::ofstream outMTL((pathWithoutExtension+".mtl").c_str());
    outOBJ<<"mtllib "<<nameBase<<".mtl"<<std::endl;
    outOBJ<<"o "<<getName()<<std::endl;
    for(int i=0;i<VertexData.size();i++)
    {
        outOBJ<<"v "<<VertexData[i].Position.x<<" "<<VertexData[i].Position.y<<" "<<VertexData[i].Position.z<<std::endl;
        outOBJ<<"vt "<<VertexData[i].TextureCoords.x<<" "<<VertexData[i].TextureCoords.y<<" "<<std::endl;
        outOBJ<<"vn "<<VertexData[i].Normal.x<<" "<<VertexData[i].Normal.y<<" "<<VertexData[i].Normal.z<<std::endl;
    }
    outOBJ<<"usemtl "<<getName()<<"mat"<<std::endl;
    for(int i=0;i<Indices.size()/3;i++)
    {
        outOBJ<<"f "<<Indices[i*3]+1<<"/"<<Indices[i*3]+1<<"/"<<Indices[i*3]+1<<" "<<Indices[i*3+1]+1<<"/"<<Indices[i*3+1]+1<<"/"<<Indices[i*3+1]+1<<" "<<Indices[i*3+2]+1<<"/"<<Indices[i*3+2]+1<<"/"<<Indices[i*3+2]+1<<std::endl;
    }
    outOBJ.close();

    outMTL<<"newmtl "<<getName()<<"mat"<<std::endl;
    outMTL<<"Ns 10.000"<<std::endl;
    outMTL<<"Ka 1.0000 1.00000 1.00000"<<std::endl;
    outMTL<<"Kd 1.0000 1.00000 1.00000"<<std::endl;
    outMTL<<"Ks 0.0000 0.00000 0.00000"<<std::endl;
    outMTL<<"illum 2"<<std::endl;
    for(int i=0;i<Textures.size();i++)
    {
        if(MTLTextureTypeNames[Textures[i]->getTextureType()]!="X")
        {
            outMTL<<MTLTextureTypeNames[Textures[i]->getTextureType()]<<" "<<Textures[i]->getName()<<std::endl;
            Textures[i]->saveToFile(directory+Textures[i]->getName());
        }

    }
    outMTL.close();
}





/*
std::string file;
float getFromPos(int &index)
{
        std::string arg;
        for(int pos=index+1;file[pos]!=std::string::npos&&file[pos]!=',';pos++)
        {
            arg+=file[pos];
            index=pos+1;
        }
        return atof(arg.c_str());
}



Mesh* importRoblox(std::string filename)
{
    file = GLOBAL::textFileRead(filename);
    std::ifstream f(filename.c_str());
    std::string versionNumber,numTriangles;
    getline(f,versionNumber);
    getline(f,numTriangles);
    int verts =3*atoi(numTriangles.c_str());
    int index=0;
    std::vector<Vertex> Vert;
    for(int i=0;i<verts;i++)
    {
        int vStart = file.find('[',index+1);
        int nStart = file.find('[',vStart+1);
        int tStart = file.find('[',nStart+1);
        glm::vec3 pos = glm::vec3(getFromPos(vStart),getFromPos(vStart),getFromPos(vStart));
        glm::vec3 norm = glm::vec3(getFromPos(nStart),getFromPos(nStart),getFromPos(nStart));
        glm::vec2 tex = glm::vec2(getFromPos(tStart),getFromPos(tStart));
        pos = glm::vec3(pos.z,pos.y,pos.x);
        norm = glm::vec3(norm.z,norm.y,norm.x);
        tex = glm::vec2(tex.y,tex.x);
        Vert.push_back(Vertex(pos.x,pos.y,pos.z,norm.x,norm.y,norm.z,tex.x,tex.y));
        index =      file.find(']',tStart+1);

    }
    Mesh* retMesh = new Mesh();
    retMesh->meshFromVector(Vert);
    return retMesh;
}
*/
