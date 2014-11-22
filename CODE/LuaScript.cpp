#include "LuaScript.h"
int runThread(void* ptr);

lua_State *LuaScript::getState()
{
    if(L==0)
    {
        L = luaL_newstate();
        luaopen_io(L);
        luaopen_base(L);
        luaopen_table(L);
        luaopen_string(L);
        luaopen_math(L);
        luaL_openlibs(L);
    }

    return L;
}

void LuaScript::loadFile(std::string path)
{
    luaL_loadfile(getState(),path.c_str());
    isFile = true;
    source = path;
}

void LuaScript::loadString(std::string s)
{
    luaL_loadstring(getState(),s.c_str());
    source =s;
    isFile = false;
}

std::string LuaScript::Run(bool multithreaded)
{
    status="";
    if(multithreaded)
        luathread = SDL_CreateThread(runThread,"LuaThread ",this);
    else
        runThread(this);
    return status;
}
void LuaScript::Stop()
{
    //status = NULL;
}
int runThread(void* ptr)
{
    LuaScript *theScript = (LuaScript*)ptr;
    luabridge::setGlobal(theScript->getState(),theScript,"this");
    if(theScript->isFile)
    {
        if(luaL_loadfile(theScript->getState(), theScript->source.c_str())|| lua_pcall(theScript->getState(),0,0,0))
            theScript->status = lua_tostring(theScript->getState(),-1);
    }
    else
    {
        if(luaL_dostring(theScript->getState(), theScript->source.c_str()))
            theScript->status =  lua_tostring(theScript->getState(),-1);
    }
    std::cout<<theScript->status;
}
