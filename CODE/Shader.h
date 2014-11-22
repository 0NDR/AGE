#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED
#include "Resource.h"

class Shader: public Resource
{
    protected:
        void DefaultConstructor();
        size_t bufferlength = 1800;
    public:
    std::string VertexSource;
    std::string FragmentSource;
    std::string GeometrySource;
    GLuint ShaderProgram;
    GLuint VertexShader;
    GLuint FragmentShader;
    GLuint GeometryShader;

    Shader(){ DefaultConstructor(); addNewType();}
    Shader(Object* parent): Resource(parent){DefaultConstructor(); addNewType();}
    Shader(std::string name): Resource(name){DefaultConstructor(); addNewType();}
    Shader(Object* parent, std::string name): Resource(parent,name){DefaultConstructor(); addNewType();}

    void LoadAndCompileShader(std::string source, GLenum type);
    void CompileAndLinkProgram();
    void Activate();

    static std::string TypeID() {return "Shader";}
    virtual std::string type() {return "Shader";}
    static void RegisterLua(lua_State* l)
    {
        if(!GLOBAL::isRegistered(Resource::TypeID(),l))
        {
            Resource::RegisterLua(l);
        }
        GLOBAL::addRegister(Shader::TypeID(),l);
        luabridge::getGlobalNamespace(l).deriveClass<Shader,Resource>(TypeID().c_str())
                                            .addFunction("LoadShader",&Shader::LoadAndCompileShader)
                                            .addFunction("CompileAndLink",&Shader::CompileAndLinkProgram)
                                            .addFunction("Activate",&Shader::Activate)
                                        .endClass();
        luabridge::setGlobal(l,GL_VERTEX_SHADER,"GL_VERTEX_SHADER");
        luabridge::setGlobal(l,GL_FRAGMENT_SHADER,"GL_FRAGMENT_SHADER");
        luabridge::setGlobal(l,GL_GEOMETRY_SHADER,"GL_GEOMETRY_SHADER");


    }
};


#endif // RESOURCE_H_INCLUDED
