#ifndef Speaker_H_INCLUDED
#define Speaker_H_INCLUDED
#include "Object3D.h"
#include "Sound.h"
class Speaker: public Object3D
{
    private:
        ALuint source;
        Sound *buffer;
    public:
    Speaker(){addNewType(); source=-1;}
    Speaker(Object* parent): Object3D(parent){addNewType(); source=-1;}
    Speaker(std::string name): Object3D(name){addNewType(); source=-1;}
    Speaker(Object* parent, std::string name): Object3D(parent,name){addNewType(); source=-1;}


    bool isLooping();
    glm::vec3 getVelocity();
    float getRolloffFactor();
    float getPitch();
    float getGain();
    ALuint getSource();
    Sound* getBuffer();

    void setVelocity(glm::vec3 vel);
    void setRolloffFactor(float fac);
    void setPitch(float pitch);
    void setGain(float gain);
    void setBuffer(Sound* snd);

    void Play(bool loop = false);
    void Pause();
    void Stop();
    void Rewind();
    void deleteSource();
    virtual void Update();


    static std::string TypeID() {return "Speaker";}
    virtual std::string type() {return "Speaker";}

    static void RegisterLua(lua_State* l)
    {
        if(!GLOBAL::isRegistered(Object3D::TypeID(),l))
        {
            Object3D::RegisterLua(l);
        }
        GLOBAL::addRegister(Speaker::TypeID(),l);
        luabridge::getGlobalNamespace(l).deriveClass<Speaker,Object3D>(TypeID().c_str())
                                        .endClass();
    }
};


#endif // Sound_H_INCLUDED
