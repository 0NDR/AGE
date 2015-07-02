#ifndef RESOURCE_FACTORY_INCLUDED
#define RESOURCE_FACTORY_INCLUDED

#include "Resource.h"


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
        template<class t> t* loadFromFile(std::string filepath);
        int loadFromFileLua(lua_State *l);
        static std::string TypeID(){return "ResourceFactory";}
        static void RegisterLua(lua_State* l, bool InitParentType = false)
        {
            GLOBAL::addRegister(ResourceFactory::TypeID(),l);
            luabridge::getGlobalNamespace(l).beginClass<ResourceFactory>(TypeID().c_str())
                                                    .addCFunction("loadFromFile", &ResourceFactory::loadFromFileLua)
                                            .endClass();
        }

};


#endif // RESOURCE_FACTORY_INCLUDED
