#ifndef RESOURCE_H_INCLUDED
#define RESOURCE_H_INCLUDED
#include "Object.h"
class ResourceFactory;
class Resource: public Object  ///Base class for any loaded or managable data
{
public:
    Resource(){addNewType();}
    Resource(Object* parent): Object(parent){addNewType();}
    Resource(std::string name): Object(name){addNewType();}
    Resource(Object* parent, std::string name): Object(parent,name){addNewType();}
    ResourceFactory* RF;
    void setResourceFactory(ResourceFactory *newRF);
    virtual void loadFromFile(std::string file)=0; ///<Load this resource object from a file
    static std::string TypeID() {return "Resource";}///<Returns Resource's class name
    virtual std::string type() {return "Resource";}

    static void RegisterLua(lua_State *l)
    {
        if(!GLOBAL::isRegistered(Object::TypeID(),l))
        {
            Object::RegisterLua(l);
        }
        GLOBAL::addRegister(Resource::TypeID(),l);
        luabridge::getGlobalNamespace(l).deriveClass<Resource,Object>(TypeID().c_str())
                                        .endClass();
    }
};


#endif // RESOURCE_H_INCLUDED
