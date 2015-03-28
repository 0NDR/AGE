#ifndef BONEDMESH_H_INCLUDED
#define BONEDMESH_H_INCLUDED
#include "Mesh.h"
#include "BonedModel.h"
#include <map>
#define BonesPerVertex 4
struct VertexBoneData:Vertex
{
    int boneIndicies[BonesPerVertex] = {0,0,0,0};
    float Weights[BonesPerVertex] = {0,0,0,0};
};
class BonedMesh: public Mesh///<Extension of Mesh which handles meshes containing bones for animation
{
    public:
        std::vector<VertexBoneData> VertexData; ///<Vector of the actual verticies used in this mesh
        BonedMesh(){addNewType();}
        BonedMesh(Object* parent): Mesh(parent){addNewType();}
        BonedMesh(std::string name): Mesh(name){addNewType();}
        BonedMesh(Object* parent, std::string name): Mesh(parent,name){addNewType();}

        void loadFromFile(std::string filepath){;}///<Load the mesh from a file with default parameters
        void meshFromAssimp(aiMesh* m);
        void drawToShader(Shader* shdr);
        void loadBones(std::map<std::string,int> &BoneMap, std::vector<BoneInfo> &BoneInfos, int &NumBones);

        static std::string TypeID() {return "BonedMesh";}///< Returns Mesh's class name
        virtual std::string type() {return "BonedMesh";}
        static void RegisterLua(lua_State *l)
        {



            if(!GLOBAL::isRegistered(Mesh::TypeID(),l))
            {
                Mesh::RegisterLua(l);
            }
            GLOBAL::addRegister(BonedMesh::TypeID(),l);
            luabridge::getGlobalNamespace(l).deriveClass<BonedMesh,Mesh>(TypeID().c_str())
                                                .addConstructor<void (*)(std::string)>()
                                            .endClass();
    } ///<Adds the class definition to a given lua state
};

#endif // RESOURCE_H_INCLUDED
