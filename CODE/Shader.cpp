#include "Shader.h"
void Shader::DefaultConstructor()
{
}

void Shader::deleteShaderObjects()
{
    for(int i=0;i<AttachedShaders.size();i++)
    {
        glDetachShader(ShaderProgram,AttachedShaders[i].ShaderLocation);
        glDeleteShader(AttachedShaders[i].ShaderLocation);
        AttachedShaders.erase(AttachedShaders.begin()+i);
        i--;
    }
}
void Shader::deleteShaderProgram()
{
    deleteShaderObjects();
    glDeleteProgram(ShaderProgram);
}
void Shader::deleteShaderOfType(GLenum type)
{
    for(int i=0;i<AttachedShaders.size();i++)
    {
        if(AttachedShaders[i].ShaderType == type)
        {
            AttachedShaders.erase(AttachedShaders.begin()+i);
            glDetachShader(ShaderProgram,AttachedShaders[i].ShaderLocation);
            glDeleteShader(AttachedShaders[i].ShaderLocation);
            i--;
        }
    }
}
void Shader::LoadAndCompileShader(std::string source)
{
    const int numTypes = 5;
    std::string types[numTypes] = {"Vertex","Tessellation_Evaluation","Tessellation_Control","Geometry","Fragment"};
    GLenum typeIDs[numTypes] = {GL_VERTEX_SHADER,GL_TESS_EVALUATION_SHADER,GL_TESS_CONTROL_SHADER,GL_GEOMETRY_SHADER,GL_FRAGMENT_SHADER};
    std::string StartingDelimiter = "#start";
    std::string EndingDelimiter = "#end";
    for(int i=0;i<numTypes;i++)
    {
        std::size_t FirstLocation = source.find(types[i]);
        while(FirstLocation!=std::string::npos && source.substr(FirstLocation-StartingDelimiter.size(),StartingDelimiter.size())!=StartingDelimiter)
        {
            FirstLocation = source.find(types[i],FirstLocation+1);
        }
        if(FirstLocation!=std::string::npos)
        {
            std::size_t SecondLocation = source.find(types[i],FirstLocation+1);
            while(SecondLocation!=std::string::npos && source.substr(SecondLocation-EndingDelimiter.size(),EndingDelimiter.size())!=EndingDelimiter)
            {
                SecondLocation = source.find(types[i],SecondLocation+1);
            }
            if(SecondLocation!=std::string::npos)
            {
                std::string NewSource =source.substr(FirstLocation+types[i].size(),(SecondLocation-FirstLocation)-EndingDelimiter.size()-types[i].size());
                LoadAndCompileShader(NewSource,typeIDs[i]);
            }
        }
    }
}
void Shader::loadFromFile(std::string filepath)
{
    LoadAndCompileShader(GLOBAL::textFileRead(filepath));
}
void Shader::LoadAndCompileShader(std::string source, GLenum type)
{

    GLuint shader = glCreateShader(type);
    const char* charsource = (char*)source.c_str();
    glShaderSource(shader,1,&charsource,NULL);
    glCompileShader(shader);
    ShaderStruct newShader;
    newShader.ShaderSource = source;
    newShader.ShaderType = type;
    newShader.ShaderLocation = shader;
    GLint Status;
    glGetShaderiv(shader,GL_COMPILE_STATUS, &Status);
    if(Status == GL_FALSE){
        char buffer[bufferlength];
        glGetShaderInfoLog( shader, bufferlength, NULL, buffer );
        std::cout<<type<<" COMPILE ERROR: "<<buffer<<std::endl;
        glDeleteShader(shader);
    }
    else
    {
        AttachedShaders.push_back(newShader);
    }
}
void Shader::LinkProgram()
{
    glDeleteProgram(ShaderProgram);
    ShaderProgram = glCreateProgram();
    for(int i=0;i<AttachedShaders.size();i++)
        glAttachShader(ShaderProgram, AttachedShaders[i].ShaderLocation);
    glBindFragDataLocation( ShaderProgram, 0, "outColor" );
    glLinkProgram(ShaderProgram);

    GLint Status;
    glGetProgramiv(ShaderProgram,GL_LINK_STATUS, &Status);
    if(Status == GL_FALSE){
        char buffer[bufferlength];
        glGetProgramInfoLog( ShaderProgram, bufferlength, NULL, buffer );
        std::cout<<" LINK ERROR: "<<buffer<<std::endl;
    }

    glValidateProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram,GL_VALIDATE_STATUS,&Status);
    if(Status==GL_FALSE)
    {
        char buffer[bufferlength];
        glGetProgramInfoLog( ShaderProgram, bufferlength, NULL, buffer );
        std::cout<<" VALIDATE ERROR: "<<buffer<<std::endl;
    }
}
void Shader::Activate()
{
    if(ShaderProgram!=NULL&&ShaderProgram!=0)
        glUseProgram(ShaderProgram);
}
