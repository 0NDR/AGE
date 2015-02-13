#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED
#include "Resource.h"
#include "TextureBase.h"
struct MeshBuffer;
class Mesh: public Resource///<Basic class for loading *.obj, *.3ds, *.dae, *.blend, and many more
{
    private:
        aiScene *scene;
    public:
        std::vector<GLOBAL::Vertex> VertexData; ///<Vector of the actual verticies used in this mesh
        std::vector<unsigned int> Indices;      ///<Vector which holds the index of each vertex, in order
        std::vector<GLuint> Textures;           ///<Vector holding textures associated with this mesh
        std::vector<MeshBuffer> MeshDataEntries;///<Vector holding the individual GL buffers for each mesh part

        Mesh(){addNewType();}
        Mesh(Object* parent): Resource(parent){addNewType();}
        Mesh(std::string name): Resource(name){addNewType();}
        Mesh(Object* parent, std::string name): Resource(parent,name){addNewType();}

        aiScene* getScene();///<Returns the aiScene form of the mesh, holding all the mesh data
        void loadFromFile(std::string filepath){loadFromFileParameters(filepath);}///<Load the mesh from a file with default parameters
        void loadFromFileParameters(std::string filePath,unsigned int Flags = aiProcess_GenSmoothNormals | aiProcess_Triangulate  |aiProcess_FlipUVs|aiProcess_CalcTangentSpace);///<Load the mesh from a file path, with parameteres
        void meshFromVector(std::vector<GLOBAL::Vertex> Verts);///<load the mesh from a series of verticies
        void meshFromVector(std::vector<GLOBAL::Vertex> Verts, std::vector<unsigned int> inds); ///load the mesh from a series of verticies and indices

        static std::string TypeID() {return "Mesh";}///< Returns Mesh's class name
        virtual std::string type() {return "Mesh";}
        static void RegisterLua(lua_State *l)
        {



            if(!GLOBAL::isRegistered(Resource::TypeID(),l))
            {
                Resource::RegisterLua(l);
            }
            GLOBAL::addRegister(Mesh::TypeID(),l);
            luabridge::getGlobalNamespace(l).deriveClass<Mesh,Resource>(TypeID().c_str())
                                                .addConstructor<void (*)(std::string)>()
                                                .addFunction("loadFromFile",&Mesh::loadFromFile)
                                            .endClass();
    } ///<Adds the class definition to a given lua state
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
