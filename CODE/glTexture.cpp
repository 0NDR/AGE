#include "glTexture.h"
 std::string glTexture::TypeUniforms[0xC]  = {"Texture_NoType","Texture_Diffuse","Texture_Specular","Texture_Ambient",
                         "Texture_Emmissive","Texture_Normal","Texture_Height","Texture_Shininess",
                         "Texture_Opacity","Texture_Displacement","Texture_Lightmap","Texture_Reflection",
                         };

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
aiTextureType glTexture::getTextureType()
{
    return Type;
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
void glTexture::setTextureType(aiTextureType type)
{
    Type=type;
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
    glTexImage2D( Target, 0, BytesPerPixel, DisplaySurface->w, DisplaySurface->h, 0, Format, GL_UNSIGNED_BYTE, DisplaySurface->pixels );
    glGenerateMipmap(Target);
    //gluBuild2DMipmaps(Target,Format, DisplaySurface->w, DisplaySurface->h, Format, GL_UNSIGNED_BYTE, DisplaySurface->pixels);
}
void glTexture::AttachToShader(Shader* Shaderprog)
{
    glUniform1i(glGetUniformLocation(Shaderprog->ShaderProgram,UniformLocation.c_str()),UniformIndex);
}
void glTexture::AttachAs(Shader* Shaderprog,std::string tName, int tIndex)
{
    Activate();
    bindTexture();
    setUniformLocation(tIndex,tName);
    AttachToShader(Shaderprog);
}
