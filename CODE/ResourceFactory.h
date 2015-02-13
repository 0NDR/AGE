#ifndef RESOURCE_FACTORY_INCLUDED
#define RESOURCE_FACTORY_INCLUDED

#include "Resource.h"
#include "Mesh.h"
#include "Shader.h"
#include "TextureBase.h"
#include "glTexture.h"
#include "LuaScript.h"

struct resourceHolder
{
    resourceHolder(std::string p, Resource *f)
    {
        path = p;
        o = f;
    }
    std::string path;
    Resource *o;
};
class ResourceFactory
{
    private:
        std::vector<resourceHolder> loadedFiles;
    public:
        template<class t> t* loadFromFile(std::string filepath)
        {
            for(int i=0;i<loadedFiles.size();i++)
            {
                if(loadedFiles[i].path == filepath)
                    return (t*)loadedFiles[i].o;
            }
            t* returnVal = new t;
            returnVal->t::loadFromFile(filepath);
            loadedFiles.push_back(resourceHolder(filepath,returnVal));
            return (t*)returnVal;
        }

        static std::string TypeID(){return "ResourceFactory";}
        static void RegisterLua(lua_State* l, bool InitParentType = false)
        {
            GLOBAL::addRegister(ResourceFactory::TypeID(),l);
            luabridge::getGlobalNamespace(l).beginClass<ResourceFactory>(TypeID().c_str())
                                                    .addFunction("loadMesh", &ResourceFactory::loadFromFile<Mesh>)
                                                    .addFunction("loadTexture", &ResourceFactory::loadFromFile<TextureBase>)
                                            .endClass();
        }
};


#endif // RESOURCE_FACTORY_INCLUDED
