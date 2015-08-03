#ifndef INSTANCE_FACTORY_INCLUDED
#define INSTANCE_FACTORY_INCLUDED
#include "Resource.h"

class InstanceFactory: public Resource
{
    private:
        std::vector<Object*> Defaults;
        template<class t> void pushObjectOfType(lua_State *l);
    public:
        InstanceFactory(){addNewType();}
        InstanceFactory(Object* parent): Resource(parent){addNewType();}
        InstanceFactory(std::string name): Resource(name){addNewType();}
        InstanceFactory(Object* parent, std::string name): Resource(parent,name){addNewType();}
        template<class t> void setDefaultObject(t* object);
        template<class t> t* getDefaultOfType();
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






///---------------------------Template definitions must be in their header


template<class t> void InstanceFactory::setDefaultObject(t* object)
{
    for(int i=0;i<Defaults.size();i++)
    {
        if(Defaults[i]->type() == object->type())
        {
            Defaults[i] = object;
            return;
        }
    }
    t* newGuy = new t;
    memcpy(newGuy,object,sizeof(t));
    Defaults.push_back(newGuy);
}
template<class t> t* InstanceFactory::getDefaultOfType()
{
    for(int i=0;i<Defaults.size();i++)
    {
        if(Defaults[i]->type() == t::TypeID())
        {
            return dynamic_cast<t*>(Defaults[i]);
        }
    }
    return NULL;
}
#endif // INSTANCE_FACTORY_INCLUDED
