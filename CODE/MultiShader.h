#ifndef MSHADER_H_INCLUDED
#define MSHADER_H_INCLUDED
#include "Resource.h"
#include "Shader.h"
class MultiShader: public Resource
{
    private:
        std::vector<Shader> ShaderList;
        glm::vec2 Resolution;
    public:
    GLuint frameBuffer;
    GLuint texColorBuffer;
    GLuint texDepthBuffer;
    GLuint renderBufferObject;
    MultiShader(){addNewType();frameBuffer=0;texColorBuffer=0;texDepthBuffer=0;renderBufferObject=0;}
    MultiShader(Object* parent): Resource(parent){addNewType();frameBuffer=0;texColorBuffer=0;texDepthBuffer=0;renderBufferObject=0;}
    MultiShader(std::string name): Resource(name){addNewType();frameBuffer=0;texColorBuffer=0;texDepthBuffer=0;renderBufferObject=0;}
    MultiShader(Object* parent, std::string name): Resource(parent,name){addNewType();frameBuffer=0;texColorBuffer=0;texDepthBuffer=0;renderBufferObject=0;}

    void CreateFrameBuffer(glm::vec2 newSize);
    void CreateFrameBuffer(int x, int y);

    void setResolution(glm::vec2 newSize);
    void setResolution(int x, int y);
    void Activate();
    GLuint RenderToTexture(int shaderIndex, int ResX, int ResY);
    void Delete();
};


#endif // MSHADER_H_INCLUDED
