#include "InstanceFactory.h"
#include "Object.h"
#include "gameObject.h"
#include "Light.h"
#include "LuaScript.h"
#include "KeyController.h"
#include "Mesh.h"
#include "UI.h"
#include "Billboard.h"
#include "Shader.h"
template<class t> void InstanceFactory::pushObjectOfType(lua_State *l)
{
    t* nO = new t();
    t* other = getDefaultOfType<t>();
    if(other!=NULL)
    {
        memcpy(nO,other,sizeof(t));
    }
    luabridge::push(l,nO);
}
int InstanceFactory::newObject(lua_State *l)
{
    std::string Argument = luaL_checkstring(l,2);
    if(Argument == Object::TypeID())
    {
        pushObjectOfType<Object>(l);
        return 1;
    }
    else if(Argument == gameObject::TypeID())
    {
        pushObjectOfType<gameObject>(l);
        return 1;
    }
    else if(Argument == Light::TypeID())
    {
        pushObjectOfType<Light>(l);
        return 1;
    }
    else if(Argument == LuaScript::TypeID())
    {
        pushObjectOfType<LuaScript>(l);
        return 1;
    }
    else if(Argument == Mesh::TypeID())
    {
        pushObjectOfType<Mesh>(l);
        return 1;
    }
    else if(Argument == Shader::TypeID())
    {
        pushObjectOfType<Shader>(l);
        return 1;
    }
    else if(Argument == UI::TypeID())
    {
        pushObjectOfType<UI>(l);
        return 1;
    }
    else if(Argument == Billboard::TypeID())
    {
        pushObjectOfType<Billboard>(l);
        return 1;
    }
    return 0;
}
