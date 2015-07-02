#ifndef FBUFFER_H_INCLUDED
#define FBUFFER_H_INCLUDED
#include "Resource.h"
#include "Shader.h"
#include "TextureBase.h"
class FrameBuffer: public Resource
{
    private:
        glm::vec2 Resolution;
    public:
    GLuint frameBuffer;
    GLuint texColorBuffer;
    GLuint texColorBuffer2;
    GLuint texDepthBuffer;
    GLuint renderBufferObject;
    FrameBuffer(){addNewType();frameBuffer=0;texColorBuffer=0;texDepthBuffer=0;renderBufferObject=0;}
    FrameBuffer(Object* parent): Resource(parent){addNewType();frameBuffer=0;texColorBuffer=0;texDepthBuffer=0;renderBufferObject=0;}
    FrameBuffer(std::string name): Resource(name){addNewType();frameBuffer=0;texColorBuffer=0;texDepthBuffer=0;renderBufferObject=0;}
    FrameBuffer(Object* parent, std::string name): Resource(parent,name){addNewType();frameBuffer=0;texColorBuffer=0;texDepthBuffer=0;renderBufferObject=0;}
    void loadFromFile(std::string filepath){}
    void CreateFrameBuffer(glm::vec2 newSize);
    void CreateFrameBuffer(int x, int y);
    void setResolution(glm::vec2 newSize);
    void setResolution(int x, int y);
    void Activate();
    void Deactivate();
    void Delete();

    TextureBase RenderToTexture();
    TextureBase RenderToTexture(int ResX, int ResY);
};


#endif // MSHADER_H_INCLUDED
