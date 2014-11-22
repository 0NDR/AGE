#ifndef INSTANCE_FACTORY_INCLUDED
#define INSTANCE_FACTORY_INCLUDED
#include "Resource.h"

class InstanceFactory: public Resource
{
    public:
        InstanceFactory(){addNewType();}
        InstanceFactory(Object* parent): Resource(parent){addNewType();}
        InstanceFactory(std::string name): Resource(name){addNewType();}
        InstanceFactory(Object* parent, std::string name): Resource(parent,name){addNewType();}
        static std::string TypeID(){return "InstanceFactory";}
        int newObject(lua_State *l);

        static void RegisterLua(lua_State* l)
        {

            GLOBAL::addRegister(InstanceFactory::TypeID(),l);
            luabridge::getGlobalNamespace(l).beginClass<InstanceFactory>("InstanceFactory")
                                                .addCFunction("new",&InstanceFactory::newObject)
                                            .endClass();
        }
};
#endif // INSTANCE_FACTORY_INCLUDED
