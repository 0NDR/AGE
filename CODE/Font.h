#ifndef FONT_H_INCLUDED
#define FONT_H_INCLUDED
#include "Resource.h"

class Font: public Resource
{
    protected:
        TTF_Font* font;
        std::string path;
        int pointSize = 12;
    public:
    Font(){addNewType();}
    Font(Object* parent): Resource(parent){addNewType();}
    Font(std::string name): Resource(name){addNewType();}
    Font(Object* parent, std::string name): Resource(parent,name){addNewType();}

    static std::string TypeID() {return "Font";}
    virtual std::string type() {return "Font";}

    TTF_Font* getFont();
    int getPointSize();
    std::string getPath();
    bool getKerning();
    int getHinting();
    int getStyle();

    void setKerning(bool other);
    void setHinting(int other);
    void setStyle(int other);
    void setPointSize(int other);
    void setPath(std::string filepath);

    void loadFont();
    static void RegisterLua(lua_State* l)
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
