#ifndef LUA_WORLD_INCLUDED
#define LUA_WORLD_INCLUDED
#include "LuaScript.h"
typedef void (*luaRegister) (lua_State *l);
class LuaWorld    ///Class to handle operation of lua objects and interface with C++
{
private:
        lua_State *GlobalState = NULL;
        std::vector<LuaScript*> scripts;
public:
    lua_State * getState();
    void attachScript(LuaScript *script);
    void attachObject(Object* obj);
    void RegisterClass(luaRegister *theReg);
    std::string startScripts();
    void continueScripts();
    static std::string TypeID() {return "LuaWorld";}///<Returns LuaWorld's class name
    virtual std::string type() {return "LuaWorld";}
};

#endif
