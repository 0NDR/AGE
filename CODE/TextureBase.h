#ifndef TextureBase_H_INCLUDED
#define TextureBase_H_INCLUDED
#include "Resource.h"
class TextureBase: public Resource
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
        void loadRawFromFile(std::string filePath);
        void loadRawFromArray(std::vector<char> img);
        void loadRawFromArray(char* buff, int sizeofBuff);
        void loadRawFromArray(void* buff, int w, int h, int bpp);
        void saveImageToFile(std::string path);
        static std::string TypeID() {return "TextureBase";}
        virtual std::string type() {return "TextureBase";}
        static void RegisterLua(lua_State* l)
        {
            if(!GLOBAL::isRegistered(Resource::TypeID(),l))
            {
                Resource::RegisterLua(l);
            }
            GLOBAL::addRegister(TextureBase::TypeID(),l);
            luabridge::getGlobalNamespace(l).deriveClass<TextureBase,Resource>(TypeID().c_str())
                                                .addConstructor<void (*)(std::string)>()
                                                .addFunction("loadRawFromFile",&TextureBase::loadRawFromFile)
                                                .addFunction("saveImageToFile",&TextureBase::saveImageToFile)
                                            .endClass();
        }
};


#endif // TextureBase_H_INCLUDED
