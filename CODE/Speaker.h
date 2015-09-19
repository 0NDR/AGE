#ifndef Speaker_H_INCLUDED
#define Speaker_H_INCLUDED
#include "Object3D.h"
#include "Sound.h"
class Speaker: public Object3D ///Class for managing playing audio in a 3D enviorment
{
    private:
        ALuint source;
        Sound *buffer;
    public:
    Speaker(){addNewType(); source=-1;}
    Speaker(Object* parent): Object3D(parent){addNewType(); source=-1;}
    Speaker(std::string name): Object3D(name){addNewType(); source=-1;}
    Speaker(Object* parent, std::string name): Object3D(parent,name){addNewType(); source=-1;}


    bool isLooping();                   ///<Is the audio looping?
    glm::vec3 getVelocity();            ///<Returns the velocity the sound is traveling at
    float getRolloffFactor();           ///<Returns the roll off factor, sound attenuation
    float getPitch();                   ///<Returns the pitch of the sound
    float getGain();                    ///<Returns the gain of the sound
    ALuint getSource();                 ///<Get the OpenAL audio source ID
    Sound* getBuffer();                 ///<Get the audio resource associated with this speaker

    void setVelocity(glm::vec3 vel);    ///<Set the velocity of the sound
    void setRolloffFactor(float fac);   ///<Set the rolloff factor of the sound
    void setPitch(float pitch);         ///<Set the pitch
    void setGain(float gain);           ///<Set the gain
    void setBuffer(Sound* snd);         ///<Set the audio buffer associated with this speaker

    void Play(bool loop = false);       ///<Play the audio, with or without looping
    void Pause();                       ///<Pause the audio at current position
    void Stop();                        ///<Stop the audio playback completely
    void Rewind();                      ///<Rewind the audio playback to begining
    void deleteSource();                ///<Clean the audio source
    virtual void Update();


    static std::string TypeID() {return "Speaker";}
    virtual std::string type() {return "Speaker";}

    static void RegisterLua(lua_State *l)
    {
        if(!GLOBAL::isRegistered(Object3D::TypeID(),l))
        {
            Object3D::RegisterLua(l);
        }
        if(!GLOBAL::isRegistered(Sound::TypeID(),l))
        {
            Sound::RegisterLua(l);
        }
        GLOBAL::addRegister(Speaker::TypeID(),l);
        luabridge::getGlobalNamespace(l).deriveClass<Speaker,Object3D>(TypeID().c_str())
                                            .addFunction("Play",&Speaker::Play)
                                            .addFunction("Pause",&Speaker::Pause)
                                            .addFunction("Stop",&Speaker::Stop)
                                            .addFunction("Update",&Speaker::Update)
                                            .addProperty("Buffer",(Sound* (Speaker::*)()const)&Speaker::getBuffer,&Speaker::setBuffer)
                                            .addProperty("Velocity",(glm::vec3* (Speaker::*)()const)&Speaker::getVelocity,&Speaker::setVelocity)
                                            .addProperty("RollOffFactor",(float (Speaker::*)()const)&Speaker::getRolloffFactor,&Speaker::setRolloffFactor)
                                            .addProperty("Pitch",(float (Speaker::*)()const)&Speaker::getPitch,&Speaker::setPitch)
                                            .addProperty("Gain",(float (Speaker::*)()const)&Speaker::getGain,&Speaker::setGain)
                                        .endClass();
    }
};


#endif // Sound_H_INCLUDED
