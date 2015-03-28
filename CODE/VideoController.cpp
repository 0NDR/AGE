#include "VideoController.h"

Window::Window(SDL_Window* newScreen)
{
    SDLScreen = newScreen;

}
Window::Window()
{
    if(SDL_WasInit(SDL_INIT_VIDEO)!=0)
    {
        SDL_Init(SDL_INIT_VIDEO);
    }
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,8);

    SDLScreen = SDL_CreateWindow("default title",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    context = SDL_GL_CreateContext(SDLScreen);
    glewInit();
}
Window::Window(int x, int y)
{
    if(SDL_WasInit(SDL_INIT_VIDEO)!=0)
    {
        SDL_Init(SDL_INIT_VIDEO);
    }
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,8);

    SDLScreen = SDL_CreateWindow("default title",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, x, y, 0);
    context = SDL_GL_CreateContext(SDLScreen);
    glewInit();
}

Window::Window(int x, int y, int sx, int sy)
{
    if(SDL_WasInit(SDL_INIT_VIDEO)!=0)
    {
        SDL_Init(SDL_INIT_VIDEO);
    }
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,8);

    SDLScreen = SDL_CreateWindow("default title",x, y, sx, sy, 0);
    context = SDL_GL_CreateContext(SDLScreen);
    glewInit();
}
Window::Window(int x, int y, int sx, int sy, Uint32 flags)
{
    if(SDL_WasInit(SDL_INIT_VIDEO)!=0)
    {
        SDL_Init(SDL_INIT_VIDEO);
    }
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,8);

    SDLScreen = SDL_CreateWindow("Default title",x,y,sx,sy,flags);
    context = SDL_GL_CreateContext(SDLScreen);
    glewInit();
}
Window::~Window()
{
    SDL_DestroyWindow(SDLScreen);
}
void Window::makeCurrent()
{
    SDL_GL_MakeCurrent(SDLScreen,context);
    glewInit();
}
void Window::Swap()
{
    SDL_GL_SwapWindow(SDLScreen);///<Warning: lags with V-sync under some weird conditions.
}
glm::vec2 Window::getSize()
{
    int  x,y;
    SDL_GetWindowSize(SDLScreen,&x,&y);
    return glm::vec2(x,y);
}
void Window::setSize(int newx, int newy)
{
    SDL_SetWindowSize(SDLScreen, newx, newy);
    glViewport(0,0,newx,newy);
}
void Window::setSize(glm::vec2 newxy)
{
    setSize(newxy.x,newxy.y);
}
void Window::setPosition(int newx, int newy)
{
    SDL_SetWindowPosition(SDLScreen, newx, newy);
}

void Window::Minimize()
{
    SDL_MinimizeWindow(SDLScreen);
}

void Window::Maximize()
{
    SDL_MaximizeWindow(SDLScreen);
}

bool Window::getGrabbed()
{
    return getWindowFlag(SDL_WINDOW_INPUT_GRABBED);
}
void Window::setInputGrab(bool x)
{
    SDL_SetWindowGrab(SDLScreen, x?SDL_TRUE:SDL_FALSE);
}
bool Window::getBordered()
{
    return !getWindowFlag(SDL_WINDOW_BORDERLESS);
}

void Window::setWindowBordered(bool x)
{
    SDL_SetWindowBordered(SDLScreen,x?SDL_TRUE:SDL_FALSE);
}


void Window::setHidden(bool x)
{
    if(x)
    {
        SDL_ShowWindow(SDLScreen);
    }
    else
    {
        SDL_HideWindow(SDLScreen);
    }
}

void Window::setTitle(std::string newTitle)
{
    Title = newTitle;
    SDL_SetWindowTitle(SDLScreen, (char*)newTitle.c_str());
}

bool Window::getHidden()
{
    return getWindowFlag(SDL_WINDOW_HIDDEN);
}


VideoMode Window::getFullscreenMode()
{
    if(getWindowFlag(SDL_WINDOW_FULLSCREEN))
    {
        return Fullscreen;
    }
    else if(getWindowFlag(SDL_WINDOW_FULLSCREEN_DESKTOP))
    {
        return WindowedFullscreen;
    }
    return Windowed;
}

void Window::setWindowFullscreen(VideoMode f)
{
    SDL_SetWindowFullscreen(SDLScreen, (Uint32)f);
}

std::string Window::getTitle()
{
    return Title;
}
bool Window::getWindowFlag(Uint32 flag)
{
    return (getWindowFlags())&flag;
}
Uint32 Window::getWindowFlags()
{
    return SDL_GetWindowFlags(SDLScreen);
}

Uint32 Window::getID()
{
    return SDL_GetWindowID(SDLScreen);
}

SDL_Window* Window::getWindow()
{
    return SDLScreen;
}
float Window::getAspectRatio()
{
    glm::vec2 Resolution = getSize();
    return Resolution.x/Resolution.y;
}

