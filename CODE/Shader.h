#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED
#include "Resource.h"

class Shader: public Resource///Base class for a GLSL shader
{
    protected:
        void DefaultConstructor();
        size_t bufferlength = 1800;
    public:
    std::string VertexSource;   ///<Source for the vertex shader
    std::string FragmentSource; ///<Source for the fragment shader
    std::string GeometrySource; ///<Source for the geometry shader
    GLuint ShaderProgram;       ///<The linked shader program
    GLuint VertexShader;        ///<The vertex program
    GLuint FragmentShader;      ///<The fragment program
    GLuint GeometryShader;      ///<The geometry program

    Shader(){ DefaultConstructor(); addNewType();}
    Shader(Object* parent): Resource(parent){DefaultConstructor(); addNewType();}
    Shader(std::string name): Resource(name){DefaultConstructor(); addNewType();}
    Shader(Object* parent, std::string name): Resource(parent,name){DefaultConstructor(); addNewType();}
    void loadFromFile(std::string filepath){LoadAndCompileShader(filepath,GL_VERTEX_SHADER);}///<Default load a vertex shader
    void LoadAndCompileShader(std::string source, GLenum type);                              ///<Load a shader, as either GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER
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
                                            .addFunction("LoadShader",&Shader::LoadAndCompileShader)
                                            .addFunction("Link",&Shader::LinkProgram)
                                            .addFunction("Activate",&Shader::Activate)
                                        .endClass();
        luabridge::setGlobal(l,GL_VERTEX_SHADER,"GL_VERTEX_SHADER");
        luabridge::setGlobal(l,GL_FRAGMENT_SHADER,"GL_FRAGMENT_SHADER");
        luabridge::setGlobal(l,GL_GEOMETRY_SHADER,"GL_GEOMETRY_SHADER");


    }
};


#endif // RESOURCE_H_INCLUDED
