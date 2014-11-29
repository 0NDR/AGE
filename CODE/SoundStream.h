#ifndef SoundStream_H_INCLUDED
#define SoundStream_H_INCLUDED
#include "Sound.h"

class SoundStream: public Sound
{
    private:
        ALuint buffers[2];
        Mix_Music* mux;
        SDL_AudioSpec AudioSpecification;
        long bytesRead;
    public:
    SoundStream(){addNewType(); buffer=-1;buffers[0]=-1;buffers[1]=-1; Format=0;}
    SoundStream(Object* parent): Sound(parent){addNewType(); buffer=-1; buffers[0]=-1;buffers[1]=-1; Format=0;}
    SoundStream(std::string name): Sound(name){addNewType(); buffer=-1; buffers[0]=-1;buffers[1]=-1; Format=0;}
    SoundStream(Object* parent, std::string name): Sound(parent,name){addNewType(); buffer=-1; buffers[0]=-1;buffers[1]=-1; Format=0;}
        int MusicPosition = 0;

    void loadFromFile(std::string filepath);
    Mix_MusicType getFileType();
    void ReadDataTo(ALuint Buffer);
    int getNumBuffers();
    ALuint *getBuffers();
    ALenum getFormat();
    void stopStream();
    void startStream();
    void pauseStream();
    void setStreamPosition(double pos);
    double getStreamPosition();
    static std::string TypeID() {return "SoundStream";}
    virtual std::string type() {return "SoundStream";}

    static void RegisterLua(lua_State* l)
    {
        if(!GLOBAL::isRegistered(Sound::TypeID(),l))
        {
            Sound::RegisterLua(l);
        }
        GLOBAL::addRegister(SoundStream::TypeID(),l);
        luabridge::getGlobalNamespace(l).deriveClass<SoundStream,Sound>(TypeID().c_str())
                                        .endClass();
    }
};


#endif // Sound_H_INCLUDED
