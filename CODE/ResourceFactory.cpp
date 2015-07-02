#include "ResourceFactory.h"

#include "Mesh.h"
#include "Model.h"
#include "Shader.h"
#include "TextureBase.h"
#include "glTexture.h"
#include "LuaScript.h"
int ResourceFactory::loadFromFileLua(lua_State *l)
{
    std::string file =      lua_tostring(l,-1); //2nd argument
    std::string className = lua_tostring(l,-2); //1st argument
    if(className == Model::TypeID())
    {
        Model* nO = loadFromFile<Model>(file);
        luabridge::push(l,nO);
        return 1;
    }
    else if(className == Mesh::TypeID())
    {
        Mesh* nO = loadFromFile<Mesh>(file);
        luabridge::push(l,nO);
        return 1;
    }
    else if(className == Shader::TypeID())
    {
        Shader* nO = loadFromFile<Shader>(file);
        luabridge::push(l,nO);
        return 1;
    }
    else if(className == TextureBase::TypeID())
    {
        TextureBase* nO = loadFromFile<TextureBase>(file);
        luabridge::push(l,nO);
        return 1;
    }
    else if(className == glTexture::TypeID())
    {
        glTexture* nO = loadFromFile<glTexture>(file);
        luabridge::push(l,nO);
        return 1;
    }
    else if(className == LuaScript::TypeID())
    {
        LuaScript* nO = loadFromFile<LuaScript>(file);
        luabridge::push(l,nO);
        return 1;
    }
    return 0;
}

template<class t> t* ResourceFactory::loadFromFile(std::string filepath)
{
    for(int i=0;i<loadedFiles.size();i++)
    {
        if(loadedFiles[i].path == filepath)
        {
            std::cout<<"already loaded"<<std::endl;
            return dynamic_cast<t*>(loadedFiles[i].o);
        }
    }
    t* returnVal = new t;
    returnVal->t::loadFromFile(filepath);
    loadedFiles.push_back(resourceHolder(filepath,returnVal));
    return dynamic_cast<t*>(returnVal);
}
