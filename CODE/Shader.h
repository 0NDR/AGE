#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED
#include "Resource.h"
struct ShaderStruct
{
    GLuint ShaderLocation;
    std::string ShaderSource;
    GLenum ShaderType;
};
class Shader: public Resource///Base class for a GLSL shader
{
    protected:
        void DefaultConstructor();
        size_t bufferlength = 1800;
        std::vector<ShaderStruct> AttachedShaders;
    public:
    GLuint ShaderProgram;       ///<The linked shader program

    Shader(){ DefaultConstructor(); addNewType();}
    Shader(Object* parent): Resource(parent){DefaultConstructor(); addNewType();}
    Shader(std::string name): Resource(name){DefaultConstructor(); addNewType();}
    Shader(Object* parent, std::string name): Resource(parent,name){DefaultConstructor(); addNewType();}
    void loadFromFile(std::string filepath);                                                 ///<Calls LoadAndCompileShader() on the items inside a file
    void LoadAndCompileShader(std::string source);                                           ///<Load all types found in a source
    void LoadAndCompileShader(std::string source, GLenum type);                              ///<Load a shader of a given type
    void deleteShaderOfType(GLenum type);                                                    ///<Deletes and shader of a type, GL_VERTEX_SHADER,GL_FRAGMENT_SHADER, etc.
    void deleteShaderProgram();                                                              ///<Deletes all shaders regaurdless of type
    void deleteShaderObjects();                                                              ///<Deletes all shader objects and then the program
    void LinkProgram();                                                                      ///<Link all the loaded shaders into ShaderProgram
    void Activate();                                                                         ///<Set the active shader

    static std::string TypeID() {return "Shader";}
    virtual std::string type() {return "Shader";}
    static void RegisterLua(lua_State *l)
    {
        if(!GLOBAL::isRegistered(Resource::TypeID(),l))
        {
            Resource::RegisterLua(l);
        }
        GLOBAL::addRegister(Shader::TypeID(),l);
        luabridge::getGlobalNamespace(l).deriveClass<Shader,Resource>(TypeID().c_str())
                                            .addFunction("LoadShader",&Shader::loadFromFile)
                                            .addFunction("Link",&Shader::LinkProgram)
                                            .addFunction("Activate",&Shader::Activate)
                                        .endClass();
        luabridge::setGlobal(l,GL_VERTEX_SHADER,"GL_VERTEX_SHADER");
        luabridge::setGlobal(l,GL_FRAGMENT_SHADER,"GL_FRAGMENT_SHADER");
        luabridge::setGlobal(l,GL_GEOMETRY_SHADER,"GL_GEOMETRY_SHADER");


    }
};


#endif // RESOURCE_H_INCLUDED
