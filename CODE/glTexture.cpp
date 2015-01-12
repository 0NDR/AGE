#include "glTexture.h"
GLint glTexture::getBytesPerPixel()
{
    if(BytesPerPixel !=-1)
        return BytesPerPixel;
    BytesPerPixel = DisplaySurface->format->BytesPerPixel;
    return BytesPerPixel;
}
GLenum glTexture::getTextureUnit()
{
    if(TextureUnit==0)
        return GL_TEXTURE0;
    return TextureUnit;
}
GLuint glTexture::getTexture()
{
    if(Texture!=0)
        return Texture;
    glGenTextures(1,&Texture);
    return Texture;
}
GLenum glTexture::getFormat()
{
    if(Format!=0)
        return Format;
    getBytesPerPixel();
    if (BytesPerPixel == 4)     // contains an alpha channel
    {
        if (DisplaySurface->format->Rmask == 0x000000ff)
            Format = GL_RGBA;
        else
            Format = GL_BGRA;
    }
    else if (BytesPerPixel == 3)     // no alpha channel
    {
        if (DisplaySurface->format->Rmask == 0x000000ff)
            Format = GL_RGB;
        else
            Format = GL_BGR;
    }
    else if (BytesPerPixel==2)
    {
        Format = GL_RG;
    } else if (BytesPerPixel==1)
    {
        Format = GL_RED;
    }
    return Format;
}

void glTexture::setTarget(GLenum TextureTarget)
{
    Target = TextureTarget;
}
void glTexture::setTextureUnit(GLenum Unit)
{
    TextureUnit = Unit;
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
