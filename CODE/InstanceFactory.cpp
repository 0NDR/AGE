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
    t* other = getDefaultOfType<t>();///get default type, if it exists.
    luabridge::LuaRef parArgument = luabridge::LuaRef::fromStack(l,3); ///get second argument

    if(other!=NULL)
    {
        memcpy(nO,other,sizeof(t));
    }
    if(parArgument.isUserdata()) ///If 2nd arg is an object
    {
        Object* parentArg = parArgument.cast<Object*>();
        nO->setParent(parentArg);///set the object as its parent
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
