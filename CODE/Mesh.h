#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED
#include "Resource.h"
#include "glTexture.h"
#include "Shader.h"
extern  std::string MTLTextureTypeNames[0xC];
struct Vertex{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TextureCoords;
    glm::vec3 Tangent;
    glm::vec3 BiTangent;
    glm::vec3 tan1;
    glm::vec3 tan2;
    Vertex(){}
    Vertex(float Posx,float Posy,float Posz,float Normx,float Normy,float Normz,float TexX,float TexY){ //good old TexX
        Position = glm::vec3(Posx,Posy,Posz);
        Normal = glm::vec3(Normx,Normy,Normz);
        TextureCoords = glm::vec2(TexX,TexY);
    }
};
class Mesh: public Resource///<Basic class handling vertex, index, and texture data
{
    protected:
        aiMesh *AssimpMesh;
        GLuint VertexBuffer;
        GLuint IndexBuffer;
    public:
        std::vector<Vertex> VertexData; ///<Vector of the actual verticies used in this mesh
        std::vector<unsigned int> Indices;      ///<Vector which holds the index of each vertex, in order
        std::vector<glTexture*> Textures;           ///<Vector holding textures associated with this mesh

        Mesh(){addNewType();}
        Mesh(Object* parent): Resource(parent){addNewType();}
        Mesh(std::string name): Resource(name){addNewType();}
        Mesh(Object* parent, std::string name): Resource(parent,name){addNewType();}

        aiMesh* getMesh();///<Returns the aiScene form of the mesh, holding all the mesh data
        void loadFromFile(std::string filepath){;}///<Load the mesh from a file with default parameters
        void saveToFile(std::string filepath);
        void meshFromAssimp(aiMesh* m);
        void meshFromVector(std::vector<Vertex> Verts);///<load the mesh from a series of verticies
        void meshFromVector(std::vector<Vertex> Verts, std::vector<unsigned int> inds); ///load the mesh from a series of verticies and indices
        void loadTextures(std::string pathToDirectory, aiScene *scene);
        void loadTexturesFromMaterial(aiMaterial* mat, aiTextureType type, std::string dir);
        virtual void drawToShader(Shader* shdr);

        void calculateTangentsBitangents();
        static std::string TypeID() {return "Mesh";}///< Returns Mesh's class name
        virtual std::string type() {return "Mesh";}
        static void RegisterLua(lua_State *l)
        {



            if(!GLOBAL::isRegistered(Resource::TypeID(),l))
            {
                Resource::RegisterLua(l);
            }
            if(!GLOBAL::isRegistered(glTexture::TypeID(),l))
            {
                glTexture::RegisterLua(l);
            }
            GLOBAL::addRegister(Mesh::TypeID(),l);
            luabridge::getGlobalNamespace(l).deriveClass<Mesh,Resource>(TypeID().c_str())
                                                .addConstructor<void (*)(std::string)>()
                                                .addFunction("loadFromFile",&Mesh::loadFromFile)
                                            .endClass();
    } ///<Adds the class definition to a given lua state
};


#endif // RESOURCE_H_INCLUDED
