#ifndef COMBINEDCONTROLLER_H_INCLUDED
#define COMBINEDCONTROLLER_H_INCLUDED

#include "GLOBAL.h"
#include "VideoController.h"
#include "AudioController.h"
#include "KeyController.h"

class CombinedController: public Window, public AudioController, public KeyController
{
    private:

    public:
        CombinedController(){}
        CombinedController(SDL_Window *newScreen):Window(newScreen){}
        CombinedController(int x, int y):Window(x,y){}
        CombinedController(int x, int y, int sx, int sy):Window(x,y,sx,sy){}
        CombinedController(int x, int y, int sx, int sy, Uint32 flags):Window(x,y,sx,sy,flags){}

        void makeCurrent();



        static std::string TypeID() {return "KeyController";}
        virtual std::string type() {return "KeyController";}
        static void RegisterLua(lua_State* l)
        {
                 if(!GLOBAL::isRegistered(Window::TypeID(),l))
                {
                    Window::RegisterLua(l);
                }
                if(!GLOBAL::isRegistered(AudioController::TypeID(),l))
                {
                    AudioController::RegisterLua(l);
                }
                if(!GLOBAL::isRegistered(KeyController::TypeID(),l))
                {
                    KeyController::RegisterLua(l);
                }
        }

};


#endif // OBJECT_H_INCLUDED
