#ifndef VIDEOCONTROLLER_H_INCLUDED
#define VIDEOCONTROLLER_H_INCLUDED

#include "GLOBAL.h"
typedef void (*WINDOWRESIZED)(int x,int y);


enum VideoMode
{
  Windowed = 0,
  Fullscreen = SDL_WINDOW_FULLSCREEN,
  WindowedFullscreen = SDL_WINDOW_FULLSCREEN_DESKTOP
};
class Window
{
    private:
        SDL_Window *SDLScreen;
        std::string Title;

    public:
        SDL_GLContext context;
        Window(SDL_Window *newScreen);
        Window(int x, int y);
        Window(int x, int y, int sx, int sy);
        Window(int x, int y, int sx, int sy, Uint32 flags);
        ~Window();

        void setSize(int newx, int newy);
        void setSize(glm::vec2 newxy);
        void setPosition(int newx, int newy);
        void makeCurrent();
        void Swap();
        void Minimize();
        void Maximize();
        void setTitle(std::string newTitle);
        void setInputGrab(bool x);
        void setWindowBordered(bool x);
        void setHidden(bool x);
        void setWindowFullscreen(VideoMode f);
        bool getWindowFlag(Uint32 flag);
        bool getHidden();
        bool getGrabbed();
        bool getBordered();

        SDL_Window* getWindow();
        std::string getTitle();
        VideoMode getFullscreenMode();
        Uint32 getWindowFlags();
        Uint32 getID();
        glm::vec2 getSize();

        static std::string TypeID() {return "VideoController";}
        virtual std::string type() {return "VideoController";}
        static void RegisterLua(lua_State* l, bool InitParentType = false){}

};


#endif // OBJECT_H_INCLUDED
