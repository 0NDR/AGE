#ifndef TextTexture_H_INCLUDED
#define TextTexture_H_INCLUDED
#include "glTexture.h"
#include "Font.h"
#define TTF_RENDER_SOLID 0
#define TTF_RENDER_SHADED 1
#define TTF_RENDER_BLENDED 2
class TextTexture: public virtual TextureBase
{
    private:
        std::string text;
        glm::vec4 TextColor;
        glm::vec4 BackgroundColor;
        Font* font;
        SDL_Surface *TextImage;
        int RenderStyle;
    public:
        TextTexture(){addNewType();RenderStyle=0;}
        TextTexture(Object* parent): TextureBase(parent){addNewType();RenderStyle=0;}
        TextTexture(std::string name): TextureBase(name){addNewType();RenderStyle=0;}
        TextTexture(Object* parent, std::string name): TextureBase(parent,name){addNewType();RenderStyle=0;}
        ~TextTexture(){SDL_FreeSurface(TextImage);}
        glm::vec4* getTextColor();
        glm::vec4* getBackgroundColor();
        glm::vec2 getTextLength(int index=-1);
        std::string getText();
        Font* getFont();
        int getRenderingType();

        void setText(std::string other);
        void setTextColor(glm::vec4 other);
        void setBackgroundColor(glm::vec4 other);
        void setFont(Font* other);
        void setRenderingType(int other);

        void clearText();
        void loadText(bool drawover = true);
        void drawText();
        static std::string TypeID() {return "TextTexture";}
        virtual std::string type() {return "TextTexture";}
        static void RegisterLua(lua_State *l)
        {



            if(!GLOBAL::isRegistered(TextureBase::TypeID(),l))
            {
                TextureBase::RegisterLua(l);
            }
            if(!GLOBAL::isRegistered(Font::TypeID(),l))
            {
                Font::RegisterLua(l);
            }
            GLOBAL::addRegister(TextTexture::TypeID(),l);
            luabridge::getGlobalNamespace(l).deriveClass<TextTexture,TextureBase>(TypeID().c_str())
                                                .addFunction("getText",&TextTexture::getText)
                                                .addFunction("getTextColor",&TextTexture::getTextColor)
                                                .addFunction("getRenderingType",&TextTexture::getRenderingType)
                                                .addFunction("getBackgroundColor",&TextTexture::getBackgroundColor)
                                                .addFunction("getFont",&TextTexture::getFont)

                                                .addFunction("setText",&TextTexture::setText)
                                                .addFunction("setTextColor",&TextTexture::setTextColor)
                                                .addFunction("setRenderingType",&TextTexture::setRenderingType)
                                                .addFunction("setBackgroundColor",&TextTexture::setBackgroundColor)
                                                .addFunction("setFont",&TextTexture::setFont)

                                                .addFunction("clearText",&TextTexture::clearText)
                                                .addFunction("drawText",&TextTexture::drawText)
                                                .addFunction("loadText",&TextTexture::loadText)
                                                .addFunction("getTextLength",&TextTexture::getTextLength)
                                            .endClass();
            luabridge::setGlobal(l,TTF_RENDER_BLENDED,"TTF_RENDER_BLENDED");
            luabridge::setGlobal(l,TTF_RENDER_SHADED,"TTF_RENDER_SHADED");
            luabridge::setGlobal(l,TTF_RENDER_SOLID,"TTF_RENDER_SOLID");
       }
};


#endif // glTexture_H_INCLUDED
