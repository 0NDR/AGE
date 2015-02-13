#ifndef LUA_WORLD_INCLUDED
#define LUA_WORLD_INCLUDED/*
typedef void (*luaRegister) (lua_State *l);
class LuaWorld    ///Class to handle operation of lua objects and interface with C++
{
private:
        lua_State *GlobalState;
        std::vector<std::string> Registers;
public:
    void Update();
    lua_State * getState();
    bool isRegistered(std::string TypeID, lua_State *l);
    void addRegister(std::string TypeID, lua_State *l);
    static std::string TypeID() {return "LuaWorld";}///<Returns LuaWorld's class name
    virtual std::string type() {return "LuaWorld";}
    static void RegisterLua(lua_State *l);
};
*/
#endif
