#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED
#include "Resource.h"
#include "TextureBase.h"
struct MeshBuffer;
class Mesh: public Resource
{
    private:
        aiScene *scene;
    public:
        std::vector<GLOBAL::Vertex> VertexData;
        std::vector<unsigned int> Indices;
        std::vector<GLuint> Textures;
        std::vector<MeshBuffer> MeshDataEntries;

        Mesh(){addNewType();}
        Mesh(Object* parent): Resource(parent){addNewType();}
        Mesh(std::string name): Resource(name){addNewType();}
        Mesh(Object* parent, std::string name): Resource(parent,name){addNewType();}

        aiScene* getScene();
        void meshFromFile(std::string filePath,unsigned int Flags = aiProcess_GenSmoothNormals | aiProcess_Triangulate  |aiProcess_FlipUVs|aiProcess_CalcTangentSpace);
        void meshFromFileL(std::string filePath);
        void meshFromVector(std::vector<GLOBAL::Vertex> Verts);
        void meshFromVector(std::vector<GLOBAL::Vertex> Verts, std::vector<unsigned int> inds);

        static std::string TypeID() {return "Mesh";}
        virtual std::string type() {return "Mesh";}
        static void RegisterLua(lua_State* l)
        {
            if(!GLOBAL::isRegistered(Resource::TypeID(),l))
            {
                Resource::RegisterLua(l);
            }
            GLOBAL::addRegister(Mesh::TypeID(),l);
            luabridge::getGlobalNamespace(l).deriveClass<Mesh,Resource>(TypeID().c_str())
                                                .addConstructor<void (*)(std::string)>()
                                                .addFunction("meshFromFile",&Mesh::meshFromFileL)
                                            .endClass();
        }
};

struct MeshBuffer
{
    void init(const std::vector<GLOBAL::Vertex>  &Vertices,const std::vector<unsigned int> &Indices);
    ~MeshBuffer();
    unsigned int VertexBufferLength;
    unsigned int IndiciesBufferLength;
    unsigned int TextureIndex;
    GLuint VertexBuffer;
    GLuint IndexBuffer;
};

#endif // RESOURCE_H_INCLUDED
