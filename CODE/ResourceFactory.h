#ifndef RESOURCE_FACTORY_INCLUDED
#define RESOURCE_FACTORY_INCLUDED

#include "Resource.h"
#include "Mesh.h"
#include "Shader.h"
#include "TextureBase.h"
#include "glTexture.h"
#include "LuaScript.h"

struct meshHolder
{
    meshHolder(std::string p, Mesh *f)
    {
        path = p;
        o = f;
    }
    std::string path;
    Mesh *o;
};
struct textureHolder
{
    textureHolder(std::string p, glTexture f)
    {
        path = p;
        o = f;
    }
    std::string path;
    glTexture o;
};

class ResourceFactory: public Resource
{
    private:
        std::vector<meshHolder> loadedMeshList;
        std::vector<textureHolder> loadedTextureList;

    public:
        ResourceFactory(){addNewType();}
        ResourceFactory(Object* parent): Resource(parent){addNewType();}
        ResourceFactory(std::string name): Resource(name){addNewType();}
        ResourceFactory(Object* parent, std::string name): Resource(parent,name){addNewType();}
        //static std::string TypeID(){return "ResourceFactory";}
        Mesh *loadMesh(std::string path);
        glTexture loadTexture(std::string path);
        static void RegisterLua(lua_State* l, bool InitParentType = false)
        {
            GLOBAL::addRegister(ResourceFactory::TypeID(),l);
            luabridge::getGlobalNamespace(l).beginClass<ResourceFactory>("Factory")
                                                    .addFunction("newMesh", &ResourceFactory::loadMesh)
                                            .endClass();
        }
};


#endif // RESOURCE_FACTORY_INCLUDED
