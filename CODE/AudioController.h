#ifndef AUDIOCONTROLLER_H_INCLUDED
#define AUDIOCONTROLLER_H_INCLUDED

#include "GLOBAL.h"


class AudioController
{
    private:
        ALCdevice *AudioDevice;
        ALCcontext *AudioContext;
    public:
        AudioController()   {setDevice((ALCchar*)NULL); setContext((ALCint*)NULL);}
        AudioController(ALCint* attrlist)   {setDevice((ALCchar*)NULL); setContext(attrlist);}
        AudioController(ALCchar* devicename)   {setDevice(devicename); setContext((ALCint*)NULL);}
        AudioController(ALCint* attrlist, ALCchar* devicename)   {setDevice(devicename); setContext(attrlist);}

        ~AudioController(){alcDestroyContext(AudioContext); alcCloseDevice(AudioDevice);}

        void makeCurrent();
        void Suspend();
        void Process();
        void setContext(ALCcontext *con);
        void setContext(ALCint* attlist);
        void setDevice(ALCdevice *dev);
        void setDevice(ALCchar* devname);

        ALCdevice *getDevice();
        ALCcontext *getContext();
        static std::string TypeID() {return "AudioController";}
        virtual std::string type() {return "AudioController";}
        static void RegisterLua(lua_State* l, bool InitParentType = false){}

};


#endif // OBJECT_H_INCLUDED
