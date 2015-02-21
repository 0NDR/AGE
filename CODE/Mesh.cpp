#include "Mesh.h"
#include "ResourceFactory.h"
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

        loadTexturesFromMaterial(material,aiTextureType_DIFFUSE,pathToDirectory);
        loadTexturesFromMaterial(material,aiTextureType_SPECULAR,pathToDirectory);
        loadTexturesFromMaterial(material,aiTextureType_DISPLACEMENT,pathToDirectory);
    }
}
void Mesh::loadTexturesFromMaterial(aiMaterial* mat, aiTextureType type, std::string dir)
{
    for(GLuint i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        glTexture* texture;
       /* if(RF)
        {
            std::cout<<dir+"/"+std::string(str.C_Str())<<std::endl;
            texture = RF->loadFromFile<glTexture>(dir+"/"+std::string(str.C_Str()));
        }
        else
        {*/
            texture = new glTexture();
            texture->loadFromFile(dir+"/"+std::string(str.C_Str()));
        //}

        std::string TextureName;
        switch(type)
        {
        case aiTextureType_DIFFUSE:
            TextureName = MESH_DiffuseTexturePrefix;
            break;
        case aiTextureType_SPECULAR:
            TextureName = MESH_SpecularTexturePrefix;
            break;
        case aiTextureType_DISPLACEMENT:
            TextureName = MESH_NormalTexturePrefix;
            break;
        }
        std::stringstream s;
        s<<TextureName<<i;
        TextureName = s.str();
        std::cout<<Textures.size()<<" "<<(char*)TextureName.c_str()<<std::endl;
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
    glEnableVertexAttribArray(pA);
    glEnableVertexAttribArray(nA);
    glEnableVertexAttribArray(teA);
    glEnableVertexAttribArray(taA);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
    glVertexAttribPointer(pA, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex), 0);
    glVertexAttribPointer(nA, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (const GLvoid*)offsetof(Vertex,Normal));
    glVertexAttribPointer(teA, 2, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (const GLvoid*)offsetof(Vertex,TextureCoords));
    glVertexAttribPointer(taA, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (const GLvoid*)offsetof(Vertex,Tangent));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);

    glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(pA);
    glDisableVertexAttribArray(nA);
    glDisableVertexAttribArray(teA);
    glDisableVertexAttribArray(taA);
}
