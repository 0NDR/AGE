#ifndef Sound_H_INCLUDED
#define Sound_H_INCLUDED
#include "Resource.h"

class Sound: public Resource
{
    protected:
        ALuint buffer;
        Mix_Chunk* chunk;
        ALenum Format;
        std::string file;
    public:
    Sound(){addNewType(); buffer=-1; Format=0;}
    Sound(Object* parent): Resource(parent){addNewType(); buffer=-1; Format=0;}
    Sound(std::string name): Resource(name){addNewType(); buffer=-1; Format=0;}
    Sound(Object* parent, std::string name): Resource(parent,name){addNewType(); buffer=-1; Format=0;}

    ALuint getBuffer();
    int getFrequency();
    int getDepthBits();
    int getChannels();
    int getSize();
    ALenum getFormat();

    void loadFromFile(std::string filepath);
    std::string getFilePath();
    static std::string TypeID() {return "Sound";}
    virtual std::string type() {return "Sound";}
    static void RegisterLua(lua_State* l)
    {
        if(!GLOBAL::isRegistered(Resource::TypeID(),l))
        {
            Resource::RegisterLua(l);
        }
        GLOBAL::addRegister(Sound::TypeID(),l);
        luabridge::getGlobalNamespace(l).deriveClass<Sound,Resource>(TypeID().c_str())
                                        .endClass();
    }
};


#endif // Sound_H_INCLUDED
