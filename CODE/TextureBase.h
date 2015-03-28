#ifndef TextureBase_H_INCLUDED
#define TextureBase_H_INCLUDED
#include "Resource.h"
class TextureBase: public Resource///Base class for loading textures
{
    protected:
        GLenum Format;
        GLint BytesPerPixel;
        SDL_Surface *DisplaySurface;
        SDL_Surface *LoadedImage;
    public:
        TextureBase(){addNewType();Format=0; BytesPerPixel = -1;}
        TextureBase(Object* parent): Resource(parent){addNewType();Format=0; BytesPerPixel = -1;}
        TextureBase(std::string name): Resource(name){addNewType();Format=0; BytesPerPixel = -1;}
        TextureBase(Object* parent, std::string name): Resource(parent,name){addNewType();Format=0; BytesPerPixel = -1;}
        ~TextureBase(){SDL_FreeSurface(DisplaySurface);SDL_FreeSurface(LoadedImage);}
        void loadFromFile(std::string filePath);///<Load from a file
        void loadFromArray(std::vector<char> img);///<load from a vector of chars
        void loadFromArray(char* buff, int sizeofBuff);///<load from a char buffer
        void loadFromArray(void* buff, int w, int h, int bpp);///<load from a char buffer with specific qualities
        void saveToFile(std::string path); ///<save the image to a file

        GLenum getFormat();                                 ///<Returns the pixel format in: GL_RGB, GL_RGBA, GL_BGR, GL_BGRA, GL_RG, and GL_RED
        GLint getBytesPerPixel();                           ///<Returns the number of bytes per pixel
        SDL_Surface* getSurface(){return DisplaySurface;}

        static std::string TypeID() {return "TextureBase";}
        virtual std::string type() {return "TextureBase";}
        static void RegisterLua(lua_State *l)
        {



            if(!GLOBAL::isRegistered(Resource::TypeID(),l))
            {
                Resource::RegisterLua(l);
            }
            GLOBAL::addRegister(TextureBase::TypeID(),l);
            luabridge::getGlobalNamespace(l).deriveClass<TextureBase,Resource>(TypeID().c_str())
                                                .addConstructor<void (*)(std::string)>()
                                                .addFunction("loadFromFile",&TextureBase::loadFromFile)
                                                .addFunction("saveToFile",&TextureBase::saveToFile)
                                            .endClass();
    } ///<Adds the class definition to a given lua state
};


#endif // TextureBase_H_INCLUDED
