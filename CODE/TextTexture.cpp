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
    SDL_Color fg = {TextColor.r*255,TextColor.g*255,TextColor.b*255,TextColor.a*255}, bg = {BackgroundColor.r*255,BackgroundColor.g*255,BackgroundColor.b*255,BackgroundColor.a*255};
    if(TextImage!=NULL){SDL_FreeSurface(TextImage);}
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
}
void TextTexture::loadText(bool drawover)
{
    if(DisplaySurface!=NULL&&DisplaySurface!=LoadedImage&&DisplaySurface!=TextImage){
        SDL_FreeSurface(DisplaySurface);
    }

    if(TextImage!=NULL)
    {
            if(drawover&&LoadedImage!=NULL)
            {
                SDL_Surface *comb = NULL;
                comb = SDL_CreateRGBSurface(0,LoadedImage->w,LoadedImage->h,24,
                                              LoadedImage->format->Rmask, LoadedImage->format->Gmask,
                                              LoadedImage->format->Bmask, LoadedImage->format->Amask);
                if(RenderStyle == TTF_RENDER_BLENDED)
                    SDL_SetSurfaceAlphaMod(TextImage,TextColor.a*255);

                SDL_BlitSurface(LoadedImage,NULL,comb,NULL);
                SDL_Rect r;
                r.x = 0;
                r.y = 0;
                r.w=LoadedImage->w;
                r.h=LoadedImage->h;

                if(RenderStyle!=TTF_RENDER_SHADED)
                {
                    TextImage = SDL_ConvertSurface(TextImage,LoadedImage->format,0);
                    SDL_Surface *textBox;
                    textBox = SDL_CreateRGBSurface(0,TextImage->w,TextImage->h,32,0,0,0,0);
                    SDL_FillRect(textBox,NULL,SDL_MapRGBA(TextImage->format,BackgroundColor.r*255,BackgroundColor.g*255,BackgroundColor.b*255,BackgroundColor.a*255));
                    SDL_BlitSurface(TextImage,NULL,textBox,NULL);
                    TextImage = textBox;
                }

                SDL_BlitSurface(TextImage,&r,comb,NULL);
                DisplaySurface = comb;
            }
            else
            {
                SDL_Surface *comb;
                #if SDL_BYTEORDER == SDL_BIG_ENDIAN
                    comb = SDL_CreateRGBSurface(0,TextImage->w,TextImage->h,32,0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
                #else
                    comb = SDL_CreateRGBSurface(0,TextImage->w,TextImage->h,32,0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
                #endif
                SDL_BlitSurface(TextImage,NULL,comb,NULL);
                DisplaySurface = comb;
            }
    }
    else if(LoadedImage!=NULL)
    {
            SDL_Surface *comb;
            comb = SDL_CreateRGBSurface(0,LoadedImage->w,LoadedImage->h,LoadedImage->format->BitsPerPixel,
                              LoadedImage->format->Rmask, LoadedImage->format->Gmask,
                              LoadedImage->format->Bmask, LoadedImage->format->Amask);
            std::cout<<SDL_GetError()<<std::endl;
            SDL_BlitSurface(LoadedImage,NULL,comb,NULL);
            DisplaySurface = comb;
    }
    loadTexture();
}
