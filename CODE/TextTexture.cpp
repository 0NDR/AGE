#include "TextTexture.h"

glm::vec4* TextTexture::getTextColor(){return &TextColor;}
glm::vec4* TextTexture::getBackgroundColor(){return &BackgroundColor;}
glm::vec2 TextTexture::getTextLength(int index)
{
    int w,h;
    std::string str;
    if(index==-1)
        str = getText();
    else
        str = getText().substr(0,index);
    if(TTF_SizeText(getFont()->getFont(),str.c_str(),&w,&h)==0)
    {
        return glm::vec2(w,h);
    }
    else
    {
        std::cout<<SDL_GetError()<<std::endl;
        return glm::vec2(0,0);
    }
}
std::string TextTexture::getText(){return text;}
Font* TextTexture::getFont(){return font;}
int TextTexture::getRenderingType(){return RenderStyle;}

void TextTexture::setText(std::string other){text = other;}
void TextTexture::setTextColor(glm::vec4 other){TextColor = other;}
void TextTexture::setBackgroundColor(glm::vec4 other){BackgroundColor = other;}
void TextTexture::setFont(Font* other){font = other;}
void TextTexture::setRenderingType(int other){RenderStyle=other;}

void TextTexture::clearText()
{
    if(TextImage!=NULL){SDL_FreeSurface(TextImage);}
    TextImage= NULL;
    loadText();
}

void TextTexture::drawText()
{
    SDL_Color fg = {TextColor.r*255,TextColor.g*255,TextColor.b*255,TextColor.a*255}, bg = {255-TextColor.r*255,255-TextColor.g*255,255-TextColor.b*255,255-TextColor.a*255};
    if(TextImage!=NULL){SDL_FreeSurface(TextImage); TextImage=NULL;}
    switch(RenderStyle)
    {
        case TTF_RENDER_BLENDED:
            TextImage = TTF_RenderText_Blended(font->getFont(),text.c_str(),fg);
            break;
        case TTF_RENDER_SHADED:
            TextImage = TTF_RenderText_Shaded(font->getFont(),text.c_str(),fg,bg);
            break;
        case TTF_RENDER_SOLID:
            TextImage = TTF_RenderText_Solid(font->getFont(),text.c_str(),fg);
            break;
    }
    if(TextImage == NULL)
    {
        std::cout<<"ERROR: COULD NOT LOAD TEXT FOR "<<getName()<<std::endl;
        return;
    }
    else
    {
        if(RenderStyle == TTF_RENDER_SHADED)
        {
            SDL_SetColorKey(TextImage,SDL_TRUE,SDL_MapRGB(TextImage->format,bg.r,bg.g,bg.b)); //Kinda silly, that we render the background and then cancel it out
        }
        SDL_SetSurfaceAlphaMod(TextImage,TextColor.a*255);
    }
}
void TextTexture::loadText(bool drawover)
{
    if(DisplaySurface!=NULL&&DisplaySurface!=LoadedImage&&DisplaySurface!=TextImage){
        SDL_FreeSurface(DisplaySurface);
    }
    int pixeldepth = 32;
    if(TextImage!=NULL)
    {
            if(drawover&&LoadedImage!=NULL)
            {
                SDL_Surface* comb = NULL,*textBox  = NULL;
                #if SDL_BYTEORDER == SDL_BIG_ENDIAN
                    comb = SDL_CreateRGBSurface(0,LoadedImage->w,LoadedImage->h,pixeldepth,0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
                    textBox = SDL_CreateRGBSurface(0,TextImage->w,TextImage->h,pixeldepth,0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
                #else
                    comb = SDL_CreateRGBSurface(0,LoadedImage->w,LoadedImage->h,pixeldepth,0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
                    textBox = SDL_CreateRGBSurface(0,TextImage->w,TextImage->h,pixeldepth,0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
               #endif


                SDL_BlitSurface(LoadedImage,NULL,comb,NULL); //put the loaded image onto the combined image
                TextImage = SDL_ConvertSurface(TextImage,LoadedImage->format,0); //convert the characters image to the loaded format
                SDL_FillRect(textBox,NULL,SDL_MapRGBA(LoadedImage->format,BackgroundColor.r*255,BackgroundColor.g*255,BackgroundColor.b*255,BackgroundColor.a*255)); //fill the background texture
                SDL_BlitSurface(TextImage,NULL,textBox,NULL); // put the text onto its background
                SDL_FreeSurface(TextImage);
                TextImage = textBox;
                SDL_BlitSurface(TextImage,NULL,comb,NULL); //put the background onto the loaded image
                DisplaySurface = comb;
            }
            else
            {
                SDL_Surface *comb;
                #if SDL_BYTEORDER == SDL_BIG_ENDIAN
                    comb = SDL_CreateRGBSurface(0,TextImage->w,TextImage->h,pixeldepth,0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
                #else
                    comb = SDL_CreateRGBSurface(0,TextImage->w,TextImage->h,pixeldepth,0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
                #endif
                SDL_BlitSurface(TextImage,NULL,comb,NULL);
                DisplaySurface = comb;
            }
    }
    else if(drawover&&LoadedImage!=NULL)
    {
            SDL_Surface *comb;
            #if SDL_BYTEORDER == SDL_BIG_ENDIAN
                comb = SDL_CreateRGBSurface(0,LoadedImage->w,LoadedImage->h,pixeldepth,0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
            #else
                comb = SDL_CreateRGBSurface(0,LoadedImage->w,LoadedImage->h,pixeldepth,0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
            #endif
            SDL_BlitSurface(LoadedImage,NULL,comb,NULL);
            DisplaySurface = comb;
    }
    else
    {
        SDL_Surface *comb;
        #if SDL_BYTEORDER == SDL_BIG_ENDIAN
            comb = SDL_CreateRGBSurface(0,1,1,pixeldepth,0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
        #else
            comb = SDL_CreateRGBSurface(0,1,1,pixeldepth,0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
        #endif
        DisplaySurface = comb;
    }
}
