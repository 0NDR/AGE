#include "GLOBAL.h"
#include <map>
#include <string>
#include "CombinedController.h"
#include "GameShader.h"
#include "gameObject.h"
#include "Billboard.h"
#include "UI.h"
#include "ResourceFactory.h"
#include "glTexture.h"
#include "Model.h"
glm::vec3 position = glm::vec3(0,-2,0),look = glm::vec3(0,0,0);
float speed=.1;
bool locked, warped,running=true;
CombinedController *CombCont;
void key(SDL_Event e)
{

    switch(e.key.keysym.sym)
    {

        case SDLK_DOWN:
            position += speed*glm::vec3(cosf(DegreesToRadians(look.y)-GLOBAL::pi/2.0),0,sinf(DegreesToRadians(look.y)-GLOBAL::pi/2.0));
            break;
        case SDLK_UP:
            position += speed*glm::vec3(cosf(DegreesToRadians(look.y)+GLOBAL::pi/2.0),0,sinf(DegreesToRadians(look.y)+GLOBAL::pi/2.0));
            break;
        case SDLK_LEFT:
            position += speed*glm::vec3(cosf(DegreesToRadians(look.y)),0,sinf(DegreesToRadians(look.y)));
            break;
        case SDLK_RIGHT:
            position -= speed*glm::vec3(cosf(DegreesToRadians(look.y)),0,sinf(DegreesToRadians(look.y)));
            break;
        case SDLK_SPACE:
            locked=!locked;
            break;
        case SDLK_q:
            position.y+=1.f;
            break;
        case SDLK_ESCAPE:
            running=!running;
    }

}
void mouseMoved(SDL_Event e)
{
    if(locked)
    {
        if(!warped)
        {
            look.y = look.y +((float)e.motion.xrel)/10.0;
            look.x = look.x +((float)e.motion.yrel)/10.0;
            if (look.y>=360){look.y =0;}
            if (look.y<0){look.y = 360;}
                look.x = GLOBAL::Clamp(look.x, -90, 90);
        }
        else
        {
            warped = false;
        }
        if (GLOBAL::Distance(e.motion.x, CombCont->getSize().x/2, e.motion.y, CombCont->getSize().y/2, 0, 0)>150)
        {
            warped = true;
            SDL_WarpMouseInWindow(CombCont->getWindow(),CombCont->getSize().x/2, CombCont->getSize().y/2);
        }
    }
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

    GameShader RenderShader, UIShader;
    ResourceFactory RF;
    Model *MarbleModel, *RectangleUIModel;
    glTexture* textures[6];
    gameObject floor;
    UI displayUI, billboardUI;
    Billboard testBillboard;
    MarbleModel = RF.loadFromFile<Model>("C:/Users/Nick/Dropbox/Apps/AGE/Resources/3d/MarblePlane/marbleplane.obj");
    RectangleUIModel = RF.loadFromFile<Model>("C:/Users/Nick/Dropbox/Apps/AGE/Resources/3d/plane.obj");
    for(int i=0;i<6;i++)
    {
        std::stringstream s;
        s<<"C:/Users/Nick/Dropbox/Apps/AGE/Resources/grass/grass"<<i+1<<".png";
        textures[i] = RF.loadFromFile<glTexture>(s.str());
        textures[i]->setTarget(GL_TEXTURE_2D);
        textures[i]->setTextureProperty(GL_TEXTURE_WRAP_S,GL_CLAMP);
        textures[i]->setTextureProperty(GL_TEXTURE_WRAP_T,GL_CLAMP);
        textures[i]->setTextureProperty(GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
        textures[i]->setTextureProperty(GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        textures[i]->loadTexture();
    }
    UIShader.loadFromFile("C:/Users/Nick/Dropbox/Apps/AGE/Resources/Shaders/MultiShader2D.glsl");
    RenderShader.loadFromFile("C:/Users/Nick/Dropbox/Apps/AGE/Resources/Shaders/MultiShader.glsl");
    RenderShader.LinkProgram();
    UIShader.LinkProgram();






    testBillboard.setWindow(CombCont);
    testBillboard.setShader(&RenderShader);
    testBillboard.Position = glm::normalize(glm::vec3(1,1,1));
    testBillboard.setSize(glm::vec4(1,0,1,0));
    testBillboard.setLockAxis(glm::vec3(0,1,0));
    testBillboard.setRotation(0);
    floor.setWindow(CombCont);
    floor.setMesh(MarbleModel);
    floor.setSize(glm::vec3(10,1,10));
    floor.setPosition(glm::vec3(0,0,0));
    floor.TextureScale = glm::vec2(10,10);
    billboardUI.setParent(&testBillboard);
    billboardUI.setWindow(CombCont);
    billboardUI.setMesh(RectangleUIModel);
    billboardUI.setColor(glm::vec4(1,1,0,.2));
    billboardUI.setSize(glm::vec4(1,0,1,0));
    billboardUI.setPosition(glm::vec4(0,0,0,0));

    displayUI.setWindow(CombCont);
    displayUI.setMesh(RectangleUIModel);
    displayUI.setColor(glm::vec4(1,1,0,.2));
    displayUI.setSize(glm::vec4(.5,0,.5,0));
    displayUI.setPosition(glm::vec4(0,0,0,0));
    float secondsPassed = 1;
    Uint64 t = SDL_GetPerformanceCounter();
    while(running)
    {
        CombCont->CheckKeys();
        RenderShader.Activate();
        glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
        glClearColor(0,0,0,1);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
        RenderShader.setProjectionMatrix(90.0,CombCont->getAspectRatio(),.01,1000);
        RenderShader.setViewMatrix(position,look);
        RenderShader.setUniform1i("LightsOn",0);
        RenderShader.setUniform1i("TextureOn",1);
        RenderShader.setUniform1i("ZeroOn",0);
        floor.Render(&RenderShader);
        RANDOM_SEED(10);
        //std::map<float, Billboard*> sorted;
        RenderShader.setUniform2f("TextureScaling",glm::vec2(1,1));
        for(int i=0;i<1000;i++)
        {
            testBillboard.setWindow(CombCont);
            testBillboard.setShader(&RenderShader);
            testBillboard.setSize(glm::vec4(1.0,0,1.0,0)*RANDOM_INT(0,100)/100.f);
            testBillboard.setLockAxis(glm::vec3(0,1.0,0));
            testBillboard.setPosition(RANDOM_INT(-100,100)/10.f,testBillboard.Size.z,RANDOM_INT(-100,100)/10.f);
            textures[RANDOM_INT(0,5)]->AttachAs(&UIShader,"Texture_Diffuse0",0);
            billboardUI.Render(&RenderShader);
        }
        //--2D--
        UIShader.Activate();
        UIShader.setProjectionMatrix(glm::ortho(-CombCont->getSize().x,CombCont->getSize().x,-CombCont->getSize().y,CombCont->getSize().y));
        glClear(GL_DEPTH_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);
        UIShader.setUniform4f("color",glm::vec4(1.f,1.f,1.f,.2f));
        glClearColor(0,0,0,1);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set the blend function
        //displayUI.Render(&UIShader);


        CombCont->Swap();
        secondsPassed = ((float)(SDL_GetPerformanceCounter()-t)/((float)SDL_GetPerformanceFrequency()));
        if(secondsPassed<1.0/300.0)
        {
            SDL_Delay(((1.0/300.0)-secondsPassed)*1000.0);
        }
        secondsPassed = ((float)(SDL_GetPerformanceCounter()-t)/((float)SDL_GetPerformanceFrequency()));
        t = SDL_GetPerformanceCounter();
    }
    return 0;
}

