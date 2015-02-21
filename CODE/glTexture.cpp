#include "glTexture.h"
std::string glTexture::getUniformLocation()
{
    return UniformLocation;
}
GLenum glTexture::getTextureUnit()
{
    return GL_TEXTURE0+UniformIndex;
}
GLuint glTexture::getTexture()
{
    if(Texture!=0)
        return Texture;
    glGenTextures(1,&Texture);
    return Texture;
}
void glTexture::setUniformLocation(GLuint ind,std::string loc)
{
    UniformIndex = ind;
    UniformLocation = loc;
}
void glTexture::setTarget(GLenum TextureTarget)
{
    Target = TextureTarget;
}
void glTexture::setTextureProperty(GLenum pname,GLint param)
{
    bindTexture();
    glTexParameteri(Target, pname, param);
}

void glTexture::bindTexture()
{
    glBindTexture(Target, getTexture());
}
void glTexture::Activate()
{
    glActiveTexture(getTextureUnit());
}
void glTexture::loadTexture()
{
    getFormat();
    Activate();
    bindTexture();
    //glTexImage2D( Target, 0, BytesPerPixel, DisplaySurface->w, DisplaySurface->h, 0, Format, GL_UNSIGNED_BYTE, DisplaySurface->pixels );
    gluBuild2DMipmaps(Target,Format, DisplaySurface->w, DisplaySurface->h, Format, GL_UNSIGNED_BYTE, DisplaySurface->pixels);
}
void glTexture::AttachToShader(Shader* Shader)
{
    glUniform1i(glGetUniformLocation(Shader->ShaderProgram,UniformLocation.c_str()),UniformIndex);
}
