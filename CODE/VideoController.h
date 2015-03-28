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
class Window ///Class to manage a window
{
    private:
        SDL_Window *SDLScreen;
        std::string Title;

    public:
        SDL_GLContext context;
        Window();
        Window(SDL_Window *newScreen);
        Window(int x, int y);
        Window(int x, int y, int sx, int sy);
        Window(int x, int y, int sx, int sy, Uint32 flags);
        ~Window();

        void setSize(int newx, int newy);       ///<Sets the window's size (x,y) in pixels
        void setSize(glm::vec2 newxy);          ///<Sets the window's size (vec2.x, vec2.y) in pixels
        void setPosition(int newx, int newy);   ///<Sets the window's position on screen (with 0,0 being the upper left corner of the primary monitor)
        void makeCurrent();                     ///<Makes the window the active window
        void Swap();                            ///<Swap GL buffers: can lag with v-sync
        void Minimize();                        ///<Minimize the window
        void Maximize();                        ///<Maximize the window
        void setTitle(std::string newTitle);    ///<Sets the window's title
        void setInputGrab(bool x);              ///<sets whether the window takes input or not
        void setWindowBordered(bool x);         ///<Sets the window to bordered or no border
        void setHidden(bool x);                 ///<Sets whether the window is hidden or not.
        void setWindowFullscreen(VideoMode f);  ///<Sets whether the window is in fullscreen or not. f must be either Windowed(0), Fullscreen(SDL_WINDOW_FULLSCREEN), or WindowedFullscreen (SDL_WINDOW_FULLSCREEN_DESKTOP)

        bool getWindowFlag(Uint32 flag);        ///<Determine whether or not a window flag is enabled (See SDL_WINDOW flags)
        bool getHidden();                       ///<Is hidden?
        bool getGrabbed();                      ///<Is grabbed?
        bool getBordered();                     ///<Is bordered?
        SDL_Window* getWindow();                ///<Get the actual SDL_Window object
        std::string getTitle();                 ///<Get the title
        VideoMode getFullscreenMode();          ///<Determine the fullscreen mode. must be either Windowed(0), Fullscreen(SDL_WINDOW_FULLSCREEN), or WindowedFullscreen (SDL_WINDOW_FULLSCREEN_DESKTOP)
        Uint32 getWindowFlags();                ///<Returns the window's flags
        Uint32 getID();                         ///<Returns the window's ID
        float getAspectRatio();
        glm::vec2 getSize();                    ///<Get the size of the window, in pixels.

        static std::string TypeID() {return "VideoController";}
        virtual std::string type() {return "VideoController";}
        static void RegisterLua(lua_State* l, bool InitParentType = false){}

};


#endif // OBJECT_H_INCLUDED
