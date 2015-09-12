#include "KeyController.h"


void KeyController::CheckKeys()
{

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            for(int i=0;i<eventFunctions.size();i++)
            {
                if(event.type==eventFunctions[i].type)
                {
                    eventFunctions[i].f(event);
                }
            }
            for(int i=0;i<luaEventFunctions.size();i++)
            {
                if(event.type==luaTypes[i])
                {
                    luaEventFunctions[i](event);
                }
            }

            switch (event.type) {

                case SDL_MOUSEBUTTONDOWN:

                    Mouse[SDL_BUTTON(event.button.button)+2] = true;
                    break;

                case SDL_MOUSEBUTTONUP:
                    Mouse[SDL_BUTTON(event.button.button)+2] = false;
                    break;

                case SDL_MOUSEMOTION:
                    Mouse[0] = event.motion.x;
                    Mouse[1] = event.motion.y;

                    break;
                default:
                    break;
            }

        }
    // End while

}


bool KeyController::getKeyState(int keyNumber)
{
    const Uint8 *states = SDL_GetKeyboardState(NULL);
    return states[keyNumber];
}

bool KeyController::getModState(Uint32 state)
{
    return (SDL_GetModState()&state)!=0;
}

void KeyController::addEvent(EVENTFUNCTION f, Uint32 callType)
{
    EventHolder eh;
    eh.f = f;
    eh.type = callType;
    eventFunctions.push_back(eh);
}
void KeyController::addEventLua(luabridge::LuaRef r, int callType)
{

    luaEventFunctions.push_back(r);
    luaTypes.push_back(callType);
}
void KeyController::removeEvent(EVENTFUNCTION f, Uint32 calltype)
{
    for(int i=0;i<eventFunctions.size();i++)
    {
        if(eventFunctions[i].f == f && eventFunctions[i].type == calltype)
        {
            removeEvent(i);
            i--;
        }
    }
}
void KeyController::removeEvent(int index)
{
    eventFunctions.erase(eventFunctions.begin()+index);
}
