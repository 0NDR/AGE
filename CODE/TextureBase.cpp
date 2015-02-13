#include "TextureBase.h"

void TextureBase::loadFromFile(std::string filePath)
{
    std::cout<<"LOADING: "<<filePath<<std::endl;
    LoadedImage = IMG_Load((char *)(filePath).c_str());
    rawData =LoadedImage->pixels;
    rawLength =sizeof(rawData);
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
    rawData = LoadedImage->pixels;
    rawLength =sizeof(rawData);
    DisplaySurface = LoadedImage;

}

void TextureBase::loadFromArray(void* buff, int w, int h, int bpp)
{

    //if(LoadedImage!=NULL){SDL_FreeSurface(LoadedImage);}
    LoadedImage = SDL_CreateRGBSurfaceFrom(buff,w,h,bpp*8,w*bpp,0x000000ff,0x0000ff00,0x00ff0000,0xff000000);
    rawData =LoadedImage->pixels;
    rawLength =sizeof(rawData);
    DisplaySurface = LoadedImage;
}
void TextureBase::saveImageToFile(std::string path)
{
    IMG_SavePNG(DisplaySurface,path.c_str());
}
