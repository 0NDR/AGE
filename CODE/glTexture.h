#ifndef glTexture_H_INCLUDED
#define glTexture_H_INCLUDED
#include "TextureBase.h"
class glTexture: public virtual TextureBase
{
    private:
        void* rawData;
        int rawWidth;
        GLenum Format;
        GLenum TextureUnit;
        GLint BytesPerPixel;
        GLuint Texture;
    public:
        glTexture(){addNewType(); TextureUnit=0; Format=0; Texture = 0; BytesPerPixel = -1;}
        glTexture(Object* parent): TextureBase(parent){addNewType(); TextureUnit=0;  Format=0; Texture = 0; BytesPerPixel = -1;}
        glTexture(std::string name): TextureBase(name){addNewType(); TextureUnit=0;  Format=0; Texture = 0; BytesPerPixel = -1;}
        glTexture(Object* parent, std::string name): TextureBase(parent,name){addNewType(); TextureUnit=0;  Format=0; Texture = 0; BytesPerPixel = -1;}

        GLenum getFormat();
        GLenum getTextureUnit();
        GLint getBytesPerPixel();
        GLuint getTexture();

        void bindTexture();
        void Activate();
        void setTextureUnit(GLenum unit);
        void setTarget(GLenum TextureTarget);
        void setTextureProperty(GLenum pname, GLint param);
        void loadTexture();

        GLenum Target;


        static std::string TypeID() {return "glTexture";}
        virtual std::string type() {return "glTexture";}
        static void RegisterLua(lua_State* l)
        {
            if(!GLOBAL::isRegistered(TextureBase::TypeID(),l))
            {
                TextureBase::RegisterLua(l);
            }
            GLOBAL::addRegister(glTexture::TypeID(),l);
            luabridge::getGlobalNamespace(l).deriveClass<glTexture,TextureBase>(TypeID().c_str())
                                                .addConstructor<void (*)(std::string)>()
                                                .addFunction("bindTexture",&glTexture::bindTexture)
                                                .addFunction("Activate",&glTexture::Activate)
                                                .addFunction("loadTexture",&glTexture::loadTexture)
                                                .addFunction("setTextureProperty",&glTexture::setTextureProperty)
                                                .addFunction("setTextureUnit",&glTexture::setTextureUnit)
                                                .addFunction("setTarget",&glTexture::setTarget)
                                            .endClass();
            luabridge::setGlobal(l,GL_TEXTURE,"GL_TEXTURE");
            luabridge::setGlobal(l,GL_LINEAR,"GL_LINEAR");
            luabridge::setGlobal(l,GL_NEAREST,"GL_NEAREST");
            luabridge::setGlobal(l,GL_LINEAR_MIPMAP_LINEAR,"GL_LINEAR_MIPMAP_LINEAR");
            luabridge::setGlobal(l,GL_NEAREST_MIPMAP_LINEAR,"GL_NEAREST_MIPMAP_LINEAR");
            luabridge::setGlobal(l,GL_LINEAR_MIPMAP_NEAREST,"GL_LINEAR_MIPMAP_NEAREST");
            luabridge::setGlobal(l,GL_NEAREST_MIPMAP_NEAREST,"GL_NEAREST_MIPMAP_NEAREST");
            luabridge::setGlobal(l,GL_CLAMP,"GL_CLAMP");
            luabridge::setGlobal(l,GL_REPEAT,"GL_REPEAT");

            luabridge::setGlobal(l,GL_TEXTURE_MAG_FILTER,"GL_TEXTURE_MAG_FILTER");
            luabridge::setGlobal(l,GL_NEAREST_MIPMAP_LINEAR,"GL_TEXTURE_MIN_FILTER");
            luabridge::setGlobal(l,GL_TEXTURE_WRAP_S,"GL_TEXTURE_WRAP_S");
            luabridge::setGlobal(l,GL_TEXTURE_WRAP_T,"GL_TEXTURE_WRAP_T");
            luabridge::setGlobal(l,GL_TEXTURE_1D,"GL_TEXTURE_1D");
            luabridge::setGlobal(l,GL_TEXTURE_2D,"GL_TEXTURE_2D");
            luabridge::setGlobal(l,GL_TEXTURE_3D,"GL_TEXTURE_3D");
            luabridge::setGlobal(l,GL_TEXTURE_CUBE_MAP,"GL_TEXTURE_CUBE_MAP");
       }
};


#endif // glTexture_H_INCLUDED
