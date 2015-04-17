#include "GLOBAL.h"
#include "CombinedController.h"
#include "GameShader.h"
#include "UI.h"
#include "ResourceFactory.h"
#include "glTexture.h"
#include "Model.h"
glm::vec3 position = glm::vec3(0,0,0);
glm::vec2 mPosition;
CombinedController *CombCont;
void key(SDL_Event e)
{

    switch(e.key.keysym.sym)
    {

        case SDLK_RIGHT:
            position.x+=1;
            break;
        case SDLK_LEFT:
            position.x-=1;
            break;
        case SDLK_UP:
            position.y+=1;
            break;
        case SDLK_DOWN:
            position.y-=1;
            break;
        case SDLK_q:
            position.z+=1;
            break;
        case SDLK_z:
            position.z-=1;
            break;
    }

}
void mouseMoved(SDL_Event e)
{
    mPosition = glm::vec2(e.motion.x,e.motion.y);
}
void resizeViewport(SDL_Event e)
{

    if(e.window.event==SDL_WINDOWEVENT_RESIZED)
        glViewport(0,0,e.window.data1,e.window.data2);
}
int main(int argc, char *argv[])
{
    GLOBAL::Init();
    SDL_Init(SDL_INIT_EVERYTHING);
    CombCont = new CombinedController(100,100,512,512,SDL_WINDOW_OPENGL| SDL_WINDOW_RESIZABLE);

    CombCont->addEvent(key,SDL_KEYDOWN);
    CombCont->addEvent(resizeViewport,SDL_WINDOWEVENT);
    CombCont->addEvent(mouseMoved,SDL_MOUSEMOTION);

    GameShader RenderShader;
    ResourceFactory RF;
    Model* UIModel, *RectangleModel;
    glTexture* displayTexture;
    UI displayUI, containUI;
    UIModel = RF.loadFromFile<Model>("C:/Users/Nick/Dropbox/Apps/AGE/Resources/3d/circle.obj");
    RectangleModel = RF.loadFromFile<Model>("C:/Users/Nick/Dropbox/Apps/AGE/Resources/3d/plane.obj");
    displayTexture = RF.loadFromFile<glTexture>("C:/Users/Nick/Dropbox/Apps/AGE/Resources/colormap.png");

    displayTexture->setTarget(GL_TEXTURE_2D);
    displayTexture->setTextureProperty(GL_TEXTURE_WRAP_S,GL_REPEAT);
    displayTexture->setTextureProperty(GL_TEXTURE_WRAP_T,GL_REPEAT);
    displayTexture->setTextureProperty(GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    displayTexture->setTextureProperty(GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    displayTexture->loadTexture();
    RenderShader.LoadAndCompileShader(GLOBAL::textFileRead("C:/Users/Nick/Dropbox/Apps/AGE/Resources/Shaders/Default2D.vs"),GL_VERTEX_SHADER);
    RenderShader.LoadAndCompileShader(GLOBAL::textFileRead("C:/Users/Nick/Dropbox/Apps/AGE/Resources/Shaders/Default2D.fs"),GL_FRAGMENT_SHADER);
    RenderShader.LinkProgram();


    containUI.setWindow(CombCont);
    containUI.setMesh(RectangleModel);
    containUI.setSize(glm::vec4(0,128,0,256));
    containUI.setPosition(glm::vec4(-1,128,1,-256));

    displayUI.setParent(&containUI);
    displayUI.setMesh(RectangleModel);
    displayUI.setWindow(CombCont);
    float velocityOfCube = .01;
    float secondsPassed = 1;
    Uint64 t = SDL_GetPerformanceCounter();
    while(true)
    {
        CombCont->CheckKeys();
        glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
        glClearColor(0,0,0,1);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set the blend function
        RenderShader.setProjectionMatrix(glm::ortho(-CombCont->getSize().x,CombCont->getSize().x,-CombCont->getSize().y,CombCont->getSize().y));
        displayTexture->AttachAs(&RenderShader,"disptext",0);
        float speed = velocityOfCube/(secondsPassed);
        containUI.Position+=glm::vec4(0,(-containUI.Position.y+mPosition.x*2)/speed,0,(-containUI.Position.w-mPosition.y*2)/speed);
        displayUI.Size = glm::vec4(.5,0,.5,0);
        containUI.setRotation(-position.z);
        RenderShader.setUniform4f("color",glm::vec4(0,1,0,1));
        containUI.Render(&RenderShader);
        RenderShader.setUniform4f("color",glm::vec4(1,0,0,1));
        displayUI.Render(&RenderShader);
        CombCont->Swap();
        secondsPassed = ((float)(SDL_GetPerformanceCounter()-t)/((float)SDL_GetPerformanceFrequency()));
        if(secondsPassed<1.0/150.0)
        {
            SDL_Delay(((1.0/150.0)-secondsPassed)*1000.0);
        }
        secondsPassed = ((float)(SDL_GetPerformanceCounter()-t)/((float)SDL_GetPerformanceFrequency()));
        t = SDL_GetPerformanceCounter();
    }
    return 0;
}

