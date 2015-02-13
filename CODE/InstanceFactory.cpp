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
int InstanceFactory::newObject(lua_State *l)
{
    std::string Argument = lua_tostring(l,-1);
    if(Argument == Object::TypeID())
    {
        Object* nO = new Object();
        luabridge::push(l,nO);
        return 1;
    }
    else if(Argument == gameObject::TypeID())
    {
        gameObject* nO = new gameObject();
        nO->Scale = glm::vec3(1,1,1);
        nO->Color = glm::vec4(1,1,1,1);
        luabridge::push(l,nO);
        return 1;
    }
    else if(Argument == Light::TypeID())
    {
        Light* nO = new Light();
        nO->DiffuseColor = glm::vec4(1,1,1,1);
        nO->AmbientColor = glm::vec4(0,0,0,1);
        nO->SpecularColor = glm::vec4(1,1,1,1);
        nO->SpotCutoff =180;
        nO->LinearAttenuation = 1;
        nO->QuadraticAttenuation =0;
        nO->ConstantAttenuation=0;
        luabridge::push(l,nO);
        return 1;
    }
    else if(Argument == LuaScript::TypeID())
    {
        LuaScript* nO = new LuaScript();
        luabridge::push(l,nO);
        return 1;
    }
    else if(Argument == Mesh::TypeID())
    {
        Mesh *nO = new Mesh();
        nO->loadFromFile("C:/Users/Nick/Dropbox/Apps/AGE/Resources/3d/sphere.obj");
        luabridge::push(l,nO);
        return 1;
    }
    else if(Argument == Shader::TypeID())
    {
        Shader *nO = new Shader();
        luabridge::push(l,nO);
        return 1;
    }
    else if(Argument == UI::TypeID())
    {
        UI *nO = new UI();
        nO->setSize(glm::vec4(1,0,1,0));
        nO->setPosition(glm::vec4(0,0,0,0));
        nO->Color = glm::vec4(1,1,1,1);
        luabridge::push(l,nO);
        return 1;
    }
    else if(Argument == Billboard::TypeID())
    {
        Billboard *nO = new Billboard();
        nO->Position = glm::vec3(0,10,0);
        nO->Size = glm::vec4(0,64,0,64);
        nO->Color = glm::vec4(1,0,0,1);
        luabridge::push(l,nO);
        return 1;
    }
    return 0;
}
