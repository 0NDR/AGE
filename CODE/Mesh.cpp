#include "Mesh.h"
#include "glTexture.h"

aiScene* Mesh::getScene()
{
    return scene;
}
void Mesh::meshFromVector(std::vector<GLOBAL::Vertex> Verts)
{
    std::vector<unsigned int> temp;
    for(int i=0;i<Verts.size();i++)
    {
        temp.push_back(i);
    }
    meshFromVector(Verts,temp);
}

void Mesh::meshFromVector(std::vector<GLOBAL::Vertex> Verts, std::vector<unsigned int> inds)
{
    scene = NULL;
    MeshDataEntries.resize(1);
    MeshDataEntries[0].TextureIndex = 0;
    VertexData = Verts;
    Indices = inds;
    MeshDataEntries[0].init(VertexData,Indices);
}

void Mesh::loadFromFileParameters(std::string filePath, unsigned int Flags )
{
    scene = (aiScene *)aiImportFile(filePath.c_str(), Flags);
    if(scene==NULL){std::cout<<"File not found"<<std::endl;return;}

    Textures.resize(scene->mNumMaterials);
    MeshDataEntries.resize(scene->mNumMeshes);
    VertexData.clear();
    Indices.clear();
    for(int x=0;x<MeshDataEntries.size();++x){ //Go through all the meshes
        const aiMesh* Mesh = scene->mMeshes[x];
        MeshDataEntries[x].TextureIndex = Mesh->mMaterialIndex; //set its texture to the index of the meshes texture.
        const aiVector3D Zero3D(0,0,0);
        for(int i=0;i<Mesh->mNumVertices;++i){
            //create the vectors to get data from
            const aiVector3D* Position = &(Mesh->mVertices[i]);
            const aiVector3D* Normal;
            const aiVector3D* TexCoord;
            const aiVector3D* bitangents;
            const aiVector3D* tangents;

            if(Mesh->HasTextureCoords(0)) //Add texture coordinates
            {
                TexCoord =&(Mesh->mTextureCoords[0][i]);
            }
            else
            {
                TexCoord =&Zero3D;
            }

            if(Mesh->HasNormals())  //Add normal coordinates
            {
                Normal = &(Mesh->mNormals[i]);
            }
            else
            {
                Normal = &Zero3D;
            }

            if(Mesh->HasTangentsAndBitangents()) //Add tangents and biTangents.
            {
                tangents = &(Mesh->mTangents[i]);
                bitangents = &(Mesh->mBitangents[i]);
            }
            else
            {
                tangents = &Zero3D;
                bitangents = &Zero3D;
            };

            GLOBAL::Vertex newVector; //Create vertex to add to it

            newVector.Position = glm::vec3(Position->x,Position->y,Position->z);          //convert pos
            newVector.TextureCoords = glm::vec2(TexCoord->x,TexCoord->y);                 //conver texture coords
            newVector.Normal = glm::vec3(Normal->x,Normal->y,Normal->z);                  //convert normals
            newVector.Tangent = glm::vec3(tangents->x,tangents->y,tangents->z);           //convert tangentz
            newVector.Bitangent = glm::vec3(bitangents->x,bitangents->y,bitangents->z);   //convert bitangents
            VertexData.push_back(newVector);                                                //add the new vertex to the vector
        }
        for (unsigned int f = 0 ; f < Mesh->mNumFaces ; f++)
        {
            const aiFace& Face = Mesh->mFaces[f];
            if(Face.mNumIndices == 3)
            {
                Indices.push_back(Face.mIndices[0]);
                Indices.push_back(Face.mIndices[1]);
                Indices.push_back(Face.mIndices[2]);
            }
        }
        MeshDataEntries[x].init(VertexData, Indices);
    }
    //MESH DATA LOADED, NOW TEXTURES

    std::string::size_type SlashIndex = filePath.find_last_of("/");
    std::string Dir;

    if (SlashIndex == std::string::npos) {
        Dir = ".";
    }
    else if (SlashIndex == 0) {
        Dir = "/";
    }
    else {
        Dir = filePath.substr(0, SlashIndex);
    }
    // Initialize the materials
    Textures.resize(scene->mNumMaterials);
    for (unsigned int i = 0 ; i < scene->mNumMaterials ; i++) {
        const aiMaterial* pMaterial = scene->mMaterials[i];

        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString Path;

            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                std::string FullPath = Dir + "/" + Path.data;
                std::cout<<"Loading "<<FullPath<<std::endl;
                glTexture newImage;
                newImage.loadFromFile(FullPath);
                newImage.loadTexture();
                Textures[i] = newImage.getTexture();
            }
        }
    }
}








void MeshBuffer::init(const std::vector<GLOBAL::Vertex>  &Vertices,const std::vector<unsigned int> &Indices)
{
    IndiciesBufferLength = Indices.size();
    VertexBufferLength = Vertices.size();
    glGenBuffers(1, &VertexBuffer);
  	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLOBAL::Vertex)*VertexBufferLength,&Vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &IndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*IndiciesBufferLength, &Indices[0], GL_STATIC_DRAW);
}

MeshBuffer::~MeshBuffer()
{
    if (VertexBuffer != 0)
    {
        glDeleteBuffers(1, &VertexBuffer);
    }

    if (IndexBuffer != 0)
    {
        glDeleteBuffers(1, &IndexBuffer);
    }
}
