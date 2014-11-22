#include "Shader.h"
void Shader::DefaultConstructor()
{
}


void Shader::LoadAndCompileShader(std::string source, GLenum type)
{

    GLuint shader = glCreateShader(type);
    const char* charsource = (char*)source.c_str();
    glShaderSource(shader,1,&charsource,NULL);
    glCompileShader(shader);
    switch(type)
    {
        case GL_VERTEX_SHADER:
            std::cout<<"Compiling vertex shader"<<std::endl;
            glDeleteShader(VertexShader);
            VertexShader = shader;
            VertexSource = source;
            break;
        case GL_GEOMETRY_SHADER:
            std::cout<<"Compiling geometry shader"<<std::endl;
            glDeleteShader(GeometryShader);
            GeometryShader = shader;
            GeometrySource = source;
            break;
        case GL_FRAGMENT_SHADER:
            std::cout<<"Compiling fragment shader"<<std::endl;
            glDeleteShader(FragmentShader);
            FragmentShader = shader;
            FragmentSource = source;
            break;
    }
    GLint Status;
    glGetShaderiv(shader,GL_COMPILE_STATUS, &Status);
    if(Status == GL_FALSE){
        char buffer[bufferlength];
        glGetShaderInfoLog( shader, bufferlength, NULL, buffer );
        std::cout<<"COMPILE ERROR: "<<buffer<<std::endl;
        glDeleteShader(shader);
    }
}
void Shader::CompileAndLinkProgram()
{
    glDeleteProgram(ShaderProgram);
    ShaderProgram = glCreateProgram();
    glAttachShader(ShaderProgram, VertexShader);
    glAttachShader(ShaderProgram, FragmentShader);
    glAttachShader(ShaderProgram, GeometryShader);
    glLinkProgram(ShaderProgram);
    glValidateProgram(ShaderProgram);
}
void Shader::Activate()
{
    if(ShaderProgram!=NULL&&ShaderProgram!=0)
        glUseProgram(ShaderProgram);
}
