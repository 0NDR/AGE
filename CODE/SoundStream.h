#ifndef SoundStream_H_INCLUDED
#define SoundStream_H_INCLUDED
#include "Sound.h"
typedef enum
{
    AUDIOSTATE_Played,
    AUDIOSTATE_Paused,
    AUDIOSTATE_Loaded,
}AudioState;
class SoundStream: public Sound///Class to manage streaming of audio files from the hard-drive
{
    private:
        Mix_Music* mux;
        SDL_AudioSpec AudioSpecification;
        Uint32 PlayTime;
        Uint32 PauseTime;
        Uint32 TotalPauseTime;
        Uint32 currentState;
        bool loop = false;
        bool fin = true;
    public:
    ALuint buffers[2];

    SoundStream(){addNewType(); buffer=-1;buffers[0]=-1;buffers[1]=-1; Format=0;PlayTime=0;PauseTime=0;TotalPauseTime=0;}
    SoundStream(Object* parent): Sound(parent){addNewType(); buffer=-1; buffers[0]=-1;buffers[1]=-1; Format=0;PlayTime=0;PauseTime=0;TotalPauseTime=0;}
    SoundStream(std::string name): Sound(name){addNewType(); buffer=-1; buffers[0]=-1;buffers[1]=-1; Format=0;PlayTime=0;PauseTime=0;TotalPauseTime=0;}
    SoundStream(Object* parent, std::string name): Sound(parent,name){addNewType(); buffer=-1; buffers[0]=-1;buffers[1]=-1; Format=0;PlayTime=0;PauseTime=0;TotalPauseTime=0;}

    void loadFromFile(std::string filepath);
    Mix_MusicType getFileType();                ///<Get the filetype, *.ogg, *.flac, *.mp3, etc
    void ReadDataTo(ALuint Buffer);             ///<Read the next chunk to an audio buffer
    int getNumBuffers();                        ///<Get the number of buffers managed by this class
    ALuint *getBuffers();                       ///<Return the array of buffers
    ALenum getFormat();                         ///<Get the audio format
    void stopStream();                          ///<Stop the audio stream entirely
    void startStream(bool looper=false);        ///<Start the audio stream, with or without looping
    void pauseStream();                         ///<Pause the stream to be restarted with startStream
    void setStreamPosition(double pos);         ///<Sets the stream position in seconds
    double getStreamPosition();                 ///<Gets the stream position in seconds
    double getStreamLength();                   ///<Get the length of the audio stream in seconds
    Uint32 getState();                          ///<Returns the audio state (played, paused, loaded)
    bool isFinished();                          ///<Returns true if the audio stream has finished playing
    static std::string TypeID() {return "SoundStream";}
    virtual std::string type() {return "SoundStream";}

    static void RegisterLua(lua_State *l)
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
