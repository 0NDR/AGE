#ifndef FONT_H_INCLUDED
#define FONT_H_INCLUDED
#include "Resource.h"

class Font: public Resource ///Used for managing fonts
{
    protected:
        TTF_Font* font;
        std::string path;
        int pointSize;
    public:
    Font(){addNewType();pointSize = 12;}
    Font(Object* parent): Resource(parent){addNewType();pointSize = 12;}
    Font(std::string name): Resource(name){addNewType();pointSize = 12;}
    Font(Object* parent, std::string name): Resource(parent,name){addNewType();pointSize = 12;}

    static std::string TypeID() {return "Font";}
    virtual std::string type() {return "Font";}

    TTF_Font* getFont();            ///<Get the internal SDL font
    int getPointSize();             ///<Get the pointsize used to draw
    std::string getPath();          ///<Get the filepath associated with this font
    bool getKerning();              ///<Get whether kerning is turned on or not
    int getHinting();               ///<Get the hinting style, one of TTF_HINTING_NORMAL, TTF_HINTING_LIGHT, TTF_HINTING_MONO, TTF_HINTING_NONE, or default TTF_HINTING_NORMAL
    int getStyle();                 ///<Get the font style, one of TTF_STYLE_BOLD, TTF_STYLE_ITALIC, TTF_STYLE_UNDERLINE, TTF_STYLE_STRIKETHROUGH, or default TTF_STYLE_NORMAL

    void setKerning(bool other);    ///<Turn on or off kerning
    void setHinting(int other);     ///<Set the hinting style, one of TTF_HINTING_NORMAL, TTF_HINTING_LIGHT, TTF_HINTING_MONO, TTF_HINTING_NONE, or default TTF_HINTING_NORMAL
    void setStyle(int other);       ///<Set the font style, one of TTF_STYLE_BOLD, TTF_STYLE_ITALIC, TTF_STYLE_UNDERLINE, TTF_STYLE_STRIKETHROUGH, or default TTF_STYLE_NORMAL
    void setPointSize(int other);   ///<Set the pointsize used to draw
    void loadFromFile(std::string filepath);    ///<load the font from a file
    static void RegisterLua(lua_State *l)
    {
        if(!GLOBAL::isRegistered(Resource::TypeID(),l))
        {
            Resource::RegisterLua(l);
        }
        GLOBAL::addRegister(Font::TypeID(),l);
        luabridge::getGlobalNamespace(l).deriveClass<Font,Resource>(TypeID().c_str())
                                                .addFunction("getPointSize",&Font::getPointSize)
                                                .addFunction("getPath",&Font::getPath)
                                                .addFunction("getKerning",&Font::getKerning)
                                                .addFunction("getHinting",&Font::getHinting)
                                                .addFunction("getStyle",&Font::getStyle)

                                                .addFunction("setPointSize",&Font::setPointSize)
                                                .addFunction("setKernins",&Font::setKerning)
                                                .addFunction("setHintins",&Font::setHinting)
                                                .addFunction("setStyle",&Font::setStyle)
                                        .endClass();
        luabridge::setGlobal(l,TTF_HINTING_NORMAL,"TTF_HINTING_NORMAL");
        luabridge::setGlobal(l,TTF_HINTING_LIGHT,"TTF_HINTING_LIGHT");
        luabridge::setGlobal(l,TTF_HINTING_MONO,"TTF_HINTING_MONO");
        luabridge::setGlobal(l,TTF_HINTING_NONE,"TTF_HINTING_NONE");

        luabridge::setGlobal(l,TTF_STYLE_BOLD,"TTF_STYLE_BOLD");
        luabridge::setGlobal(l,TTF_STYLE_NORMAL,"TTF_STYLE_NORMAL");
        luabridge::setGlobal(l,TTF_STYLE_ITALIC,"TTF_STYLE_ITALIC");
        luabridge::setGlobal(l,TTF_STYLE_UNDERLINE,"TTF_STYLE_UNDERLINE");
        luabridge::setGlobal(l,TTF_STYLE_STRIKETHROUGH,"TTF_STYLE_STRIKETHROUGH");

    }
};


#endif // RESOURCE_H_INCLUDED
