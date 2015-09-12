#ifndef KeyController_H_INCLUDED
#define KeyController_H_INCLUDED

#include "GLOBAL.h"
typedef void (*NUMCHANGED)(int KEY);
typedef void (*MOUSEMOVED)(int NewX,int NewY);
typedef void (*EVENTFUNCTION)(SDL_Event event);

struct EventHolder
{
    EVENTFUNCTION f;
    Uint32 type;
};

class KeyController
{
    private:
        std::vector<EventHolder> eventFunctions;
        std::vector<luabridge::LuaRef> luaEventFunctions;
        std::vector<Uint32> luaTypes;
    public:

        void CheckKeys();
        bool getKeyState(int keyNumber);
        bool getModState(Uint32 state);
        float Mouse[5];

        void addEvent(EVENTFUNCTION f, Uint32 callType);
        void addEventLua(luabridge::LuaRef r, int type);
        void removeEvent(EVENTFUNCTION f, Uint32 callType);
        void removeEvent(int index);
        std::vector<NUMCHANGED> KeyDownFunctions;
        std::vector<NUMCHANGED> KeyUpFunctions;
        std::vector<NUMCHANGED> MouseUpFunctions;
        std::vector<NUMCHANGED> MouseDownFunctions;
        std::vector<MOUSEMOVED> MouseMovedFunctions;
        static std::string TypeID() {return "KeyController";}
        virtual std::string type() {return "KeyController";}
        static void RegisterLua(lua_State* l)
        {
            luabridge::getGlobalNamespace(l)
                                        .beginClass<SDL_Keysym>("KeySym")
                                            .addData("sym",&SDL_Keysym::sym)
                                            .addData("scancode",&SDL_Keysym::scancode)
                                            .addData("mod",&SDL_Keysym::mod)
                                        .endClass()
                                        .beginClass<SDL_KeyboardEvent>("KeyEvent")
                                            .addData("keysym",&SDL_KeyboardEvent::keysym)
                                            .addData("type",&SDL_KeyboardEvent::type)
                                            .addData("repeat",&SDL_KeyboardEvent::repeat)
                                            .addData("state",&SDL_KeyboardEvent::state)
                                            .addData("windowID",&SDL_KeyboardEvent::windowID)
                                            .addData("timestamp",&SDL_KeyboardEvent::timestamp)
                                        .endClass()
                                        .beginClass<SDL_MouseButtonEvent>("MouseButtonEvent")
                                            .addData("button",&SDL_MouseButtonEvent::button)
                                            .addData("clicks",&SDL_MouseButtonEvent::clicks)
                                            .addData("type",&SDL_MouseButtonEvent::type)
                                            .addData("x",&SDL_MouseButtonEvent::x)
                                            .addData("y",&SDL_MouseButtonEvent::y)
                                            .addData("state",&SDL_MouseButtonEvent::state)
                                            .addData("which",&SDL_MouseButtonEvent::which)
                                            .addData("windowID",&SDL_MouseButtonEvent::windowID)
                                            .addData("timestamp",&SDL_MouseButtonEvent::timestamp)
                                        .endClass()
                                        .beginClass<SDL_MouseMotionEvent>("MouseMotionEvent")
                                            .addData("x",&SDL_MouseMotionEvent::x)
                                            .addData("y",&SDL_MouseMotionEvent::y)
                                            .addData("xrel",&SDL_MouseMotionEvent::xrel)
                                            .addData("yrel",&SDL_MouseMotionEvent::yrel)
                                            .addData("state",&SDL_MouseMotionEvent::state)
                                            .addData("type",&SDL_MouseMotionEvent::type)
                                            .addData("which",&SDL_MouseMotionEvent::which)
                                            .addData("windowID",&SDL_MouseMotionEvent::windowID)
                                            .addData("timestamp",&SDL_MouseMotionEvent::timestamp)
                                        .endClass()
                                        .beginClass<SDL_Event>("Event")
                                            .addData("key",&SDL_Event::key)
                                            .addData("motion",&SDL_Event::motion)
                                            .addData("button",&SDL_Event::button)
                                        .endClass()
                                        .beginClass<KeyController>("KeyController")
                                            .addFunction("getKeyState", &KeyController::getKeyState)
                                            .addFunction("getModState", &KeyController::getModState)
                                            .addFunction("addEvent",&KeyController::addEventLua)
                                        .endClass();
            luabridge::setGlobal(l,(int)SDL_KEYDOWN,"EVENT_KeyDown");
            luabridge::setGlobal(l,(int)SDL_KEYUP,"EVENT_KeyUp");
            luabridge::setGlobal(l,(int)SDL_MOUSEMOTION,"EVENT_MouseMotion");
            luabridge::setGlobal(l,(int)SDL_MOUSEBUTTONDOWN,"EVENT_MouseButtonDown");
            luabridge::setGlobal(l,(int)SDL_MOUSEBUTTONUP,"EVENT_MouseButtonUp");

            luabridge::setGlobal(l,(int)SDLK_LSHIFT,"KEY_LSHIFT");
            luabridge::setGlobal(l,(int)SDLK_RSHIFT,"KEY_RSHIFT");
            luabridge::setGlobal(l,(int)SDLK_RCTRL,"KEY_RCTRL");
            luabridge::setGlobal(l,(int)SDLK_LCTRL,"KEY_LCTRL");
            luabridge::setGlobal(l,(int)SDLK_BACKSPACE,"KEY_BACKSPACE");
            luabridge::setGlobal(l,(int)SDLK_UP,"KEY_UP");
            luabridge::setGlobal(l,(int)SDLK_DOWN,"KEY_DOWN");
            luabridge::setGlobal(l,(int)SDLK_LEFT,"KEY_LEFT");
            luabridge::setGlobal(l,(int)SDLK_RIGHT,"KEY_RIGHT");

            luabridge::setGlobal(l,(int)KMOD_LSHIFT,"KMOD_LSHIFT");
            luabridge::setGlobal(l,(int)KMOD_RSHIFT,"KMOD_RSHIFT");
            luabridge::setGlobal(l,(int)KMOD_LCTRL,"KMOD_LCTRL");
            luabridge::setGlobal(l,(int)KMOD_RCTRL,"KMOD_RCTRL");
            luabridge::setGlobal(l,(int)KMOD_RALT,"KMOD_RALT");
            luabridge::setGlobal(l,(int)KMOD_LALT,"KMOD_LALT");
            luabridge::setGlobal(l,(int)KMOD_ALT,"KMOD_ALT");
            luabridge::setGlobal(l,(int)KMOD_SHIFT,"KMOD_SHIFT");
            luabridge::setGlobal(l,(int)KMOD_CTRL,"KMOD_CTRL");
            luabridge::setGlobal(l,(int)KMOD_CAPS,"KMOD_CAPS");
        }
};


#endif // KeyController_H_INCLUDED
