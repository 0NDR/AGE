#ifndef RESOURCE_H_INCLUDED
#define RESOURCE_H_INCLUDED
#include "Object.h"

class Resource: public Object
{
    public:
    Resource(){addNewType();}
    Resource(Object* parent): Object(parent){addNewType();}
    Resource(std::string name): Object(name){addNewType();}
    Resource(Object* parent, std::string name): Object(parent,name){addNewType();}

    static std::string TypeID() {return "Resource";}
    virtual std::string type() {return "Resource";}

    static void RegisterLua(lua_State* l)
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
