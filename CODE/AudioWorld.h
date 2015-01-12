#ifndef AUDIOWORLD_H_INCLUDED
#define AUDIOWORLD_H_INCLUDED

#include "Renderable.h"

class AudioWorld: public Renderable
{
    AudioWorld(){addNewType(); }
    AudioWorld(Object* parent): Renderable(parent){addNewType();}
    AudioWorld(std::string name): Renderable(name){addNewType();}
    AudioWorld(Object* parent, std::string name): Renderable(parent,name){addNewType();}

    static std::string TypeID() {return "AudioWorld";}
    virtual std::string type() {return "AudioWorld";}
    static void RegisterLua(lua_State* l)
    {
            if(!GLOBAL::isRegistered(Renderable::TypeID(),l))
            {
                Renderable::RegisterLua(l);
            }
            GLOBAL::addRegister(AudioWorld::TypeID(),l);
            luabridge::getGlobalNamespace(l).deriveClass<AudioWorld,Renderable>(TypeID().c_str())
                                                .addFunction("setShader",&AudioWorld::setShader)
                                            .endClass();
    }
};


#endif // AUDIOWORLD_H_INCLUDED
