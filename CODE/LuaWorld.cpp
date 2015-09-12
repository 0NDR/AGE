#include "LuaWorld.h"

lua_State* LuaWorld::getState()
{
    if(GlobalState==NULL)
    {
        GlobalState = luaL_newstate();
        luaopen_io(GlobalState);
        luaopen_base(GlobalState);
        luaopen_table(GlobalState);
        luaopen_string(GlobalState);
        luaopen_math(GlobalState);
        luaL_openlibs(GlobalState);
    }
    return GlobalState;
}

void LuaWorld::attachScript(LuaScript* script)
{
    script->setState(getState());
    scripts.push_back(script);
}

std::string LuaWorld::startScripts()
{
    std::string outputString = "";
    for(int i=0;i<scripts.size();i++)
    {
        outputString+=scripts[i]->Run();
    }
    return outputString;
}
void LuaWorld::continueScripts()
{
    for(int i=0;i<scripts.size();i++)
    {
        scripts[i]->Continue();
    }
}
