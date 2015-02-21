#ifndef LUA_WORLD_INCLUDED
#define LUA_WORLD_INCLUDED
#include "LuaScript.h"
typedef void (*luaRegister) (lua_State *l);
class LuaWorld    ///Class to handle operation of lua objects and interface with C++
{
private:
        lua_State *GlobalState;
public:
    void Update();
    lua_State * getState();
    void attachScript(LuaScript *scrp);
    void attachObject(Object* obj);
    void RegisterClass(luaRegister *theReg);
    static std::string TypeID() {return "LuaWorld";}///<Returns LuaWorld's class name
    virtual std::string type() {return "LuaWorld";}
};

#endif
