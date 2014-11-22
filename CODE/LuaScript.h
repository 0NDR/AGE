#ifndef LUA_SCRIPT_INCLUDED
#define LUA_SCRIPT_INCLUDED
#include "Resource.h"

class LuaScript: public Resource
{
    private:
        lua_State *L;
        SDL_Thread *luathread;

    public:
        LuaScript(){addNewType(); L=0;}
        LuaScript(Object* parent): Resource(parent){addNewType(); L=0;}
        LuaScript(std::string name): Resource(name){addNewType(); L=0;}
        LuaScript(Object* parent, std::string name): Resource(parent,name){addNewType(); L=0;}
        //static std::string TypeID(){return "LuaScript";}
        std::string status;
        std::string source;
        bool isFile;
        lua_State* getState();
        void loadFile(std::string path);
        void loadString(std::string s);
        std::string Run(bool multithreaded = false);
        void Stop();
        virtual int push(lua_State *l)
        {
            luabridge::push(l,this);
            return 1;
        }

        static std::string TypeID() {return "LuaScript";}
        virtual std::string type() {return "LuaScript";}
        static void RegisterLua(lua_State* l)
        {
                if(!GLOBAL::isRegistered(Resource::TypeID(),l))
                {
                    Resource::RegisterLua(l);
                }
                GLOBAL::addRegister(LuaScript::TypeID(),l);
                luabridge::getGlobalNamespace(l).deriveClass<LuaScript,Resource>(TypeID().c_str())
                                                    .addConstructor<void (*)(std::string)>()
                                                    .addFunction("loadString",&LuaScript::loadString)
                                                    .addFunction("Run",&LuaScript::Run)
                                                    .addFunction("__eq",(bool (LuaScript::*) (const LuaScript*) const)&Object::luaIsEqual)
                                                .endClass();

        }
};

#endif
