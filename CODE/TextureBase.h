#ifndef TextureBase_H_INCLUDED
#define TextureBase_H_INCLUDED
#include "Resource.h"
class TextureBase: public Resource///Base class for loading textures
{
    protected:
        void* rawData;
        int rawLength;
        SDL_Surface *DisplaySurface;
        SDL_Surface *LoadedImage;
    public:
        TextureBase(){addNewType();}
        TextureBase(Object* parent): Resource(parent){addNewType();}
        TextureBase(std::string name): Resource(name){addNewType();}
        TextureBase(Object* parent, std::string name): Resource(parent,name){addNewType();}
        ~TextureBase(){SDL_FreeSurface(DisplaySurface);SDL_FreeSurface(LoadedImage);delete[] rawData;}
        void loadFromFile(std::string filePath);///<Load from a file
        void loadFromArray(std::vector<char> img);///<load from a vector of chars
        void loadFromArray(char* buff, int sizeofBuff);///<load from a char buffer
        void loadFromArray(void* buff, int w, int h, int bpp);///<load from a char buffer with specific qualities
        void saveImageToFile(std::string path); ///<save the image to a file
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
                                                .addFunction("saveImageToFile",&TextureBase::saveImageToFile)
                                            .endClass();
    } ///<Adds the class definition to a given lua state
};


#endif // TextureBase_H_INCLUDED
