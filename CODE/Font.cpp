#include "Font.h"

TTF_Font* Font::getFont(){return font;}
std::string Font::getPath(){return path;}
int Font::getPointSize(){return pointSize;}
bool Font::getKerning(){return TTF_GetFontKerning(font);}
int Font::getHinting(){return TTF_GetFontHinting(font);}
int Font::getStyle(){return TTF_GetFontStyle(font);}

void Font::setPointSize(int other){pointSize = other;}
void Font::setKerning(bool other){TTF_SetFontKerning(font,other?1:0);}
void Font::setHinting(int other){TTF_SetFontHinting(font,other);}
void Font::setStyle(int other){TTF_SetFontStyle(font,other);}
void Font::loadFromFile(std::string filepath)
{
    path = filepath;
    TTF_CloseFont(font);
    font = TTF_OpenFont(path.c_str(),pointSize);
}
