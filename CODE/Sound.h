#ifndef Sound_H_INCLUDED
#define Sound_H_INCLUDED
#include "Resource.h"

class Sound: public Resource  ///Base for loading audio files
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

    ALuint getBuffer();     ///<Return the OpenAL buffer associated with this audio file
    int getFrequency();     ///<Return the frequency of the audio file
    int getDepthBits();     ///<Return the depth bits (I dont actually know what that is)
    int getChannels();      ///<Return the number of channels, 1 for mono and anything else for stereo
    int getSize();          ///<Return the length of the file in bytes
    ALenum getFormat();     ///<Return the Audio Format AL_FORMAT_MONO(x) or AL_FORMAT_STEREO(x) where x is 16 or 8.

    void loadFromFile(std::string filepath);
    std::string getFilePath();
    static std::string TypeID() {return "Sound";}
    virtual std::string type() {return "Sound";}
    static void RegisterLua(lua_State *l)
    {
        if(!GLOBAL::isRegistered(Resource::TypeID(),l))
        {
            Resource::RegisterLua(l);
        }
        GLOBAL::addRegister(Sound::TypeID(),l);
        luabridge::getGlobalNamespace(l).deriveClass<Sound,Resource>(TypeID().c_str())
                                            .addFunction("getFrequency",&Sound::getFrequency)
                                            .addFunction("getDepthBits",&Sound::getDepthBits)
                                            .addFunction("getChannels",&Sound::getChannels)
                                            .addFunction("getSize",&Sound::getSize)
                                        .endClass();
    }
};


#endif // Sound_H_INCLUDED
