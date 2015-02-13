#ifndef LUA_SCRIPT_INCLUDED
#define LUA_SCRIPT_INCLUDED
#include "Resource.h"

class LuaScript: public Resource    ///Class to handle Lua scripts and lua states
{
    private:
        lua_State *L;
        SDL_Thread *luathread;
        bool multithreaded;
    public:
        LuaScript(){addNewType(); L=0;}
        LuaScript(Object* parent): Resource(parent){addNewType(); L=0;}
        LuaScript(std::string name): Resource(name){addNewType(); L=0;}
        LuaScript(Object* parent, std::string name): Resource(parent,name){addNewType(); L=0;}
        //static std::string TypeID(){return "LuaScript";}
        std::string status;
        std::string source;
        bool isFile;
        bool isMultithreaded();
        lua_State* getState();
        void loadFromFile(std::string path);
        void loadString(std::string s);
        std::string Run(bool runmultithreaded = false);
        void Stop();
        virtual int push(lua_State *l)
        {
            luabridge::push(l,this);
            return 1;
        }

        static std::string TypeID() {return "LuaScript";}
        virtual std::string type() {return "LuaScript";}
        static void RegisterLua(lua_State *l);
};

#endif
