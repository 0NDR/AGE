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
        void loadFromFile(std::string filepath){};
        int newObject(lua_State *l);

        static void RegisterLua(lua_State *l)
        {




            GLOBAL::addRegister(InstanceFactory::TypeID(),l);
            luabridge::getGlobalNamespace(l).beginClass<InstanceFactory>(TypeID().c_str())
                                                .addCFunction("new",&InstanceFactory::newObject)
                                            .endClass();
    } ///<Adds the class definition to a given lua state
};
#endif // INSTANCE_FACTORY_INCLUDED
