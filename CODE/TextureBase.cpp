#include "TextureBase.h"

void TextureBase::loadFromFile(std::string filePath)
{
    LoadedImage = IMG_Load((char *)(filePath).c_str());
    if(LoadedImage == NULL){std::cout<<"ERROR: file not found"<<std::endl;}
    DisplaySurface = LoadedImage;
}
void TextureBase::loadFromArray(std::vector<char> img)
{
    loadFromArray(&img[0],img.size());
}
void TextureBase::loadFromArray(char* buff, int sizeofBuff)
{
    //if(LoadedImage!=NULL){SDL_FreeSurface(LoadedImage);}
    SDL_RWops *rw = SDL_RWFromMem(buff,sizeofBuff);
    LoadedImage= IMG_Load_RW(rw, 1);
    if (
        LoadedImage == NULL)
    {
        printf("IMG_Load_RW: %s\n", IMG_GetError());
    }
    DisplaySurface = LoadedImage;

}
GLenum TextureBase::getFormat()
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
GLint TextureBase::getBytesPerPixel()
{
    if(BytesPerPixel !=-1)
        return BytesPerPixel;
    BytesPerPixel = DisplaySurface->format->BytesPerPixel;
    return BytesPerPixel;
}
void TextureBase::loadFromArray(void* buff, int w, int h, int bpp)
{

    //if(LoadedImage!=NULL){SDL_FreeSurface(LoadedImage);}
    LoadedImage = SDL_CreateRGBSurfaceFrom(buff,w,h,bpp*8,w*bpp,0x000000ff,0x0000ff00,0x00ff0000,0xff000000);
    DisplaySurface = LoadedImage;
}
void TextureBase::saveToFile(std::string path)
{
    IMG_SavePNG(DisplaySurface,path.c_str());
}
