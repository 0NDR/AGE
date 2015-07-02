#include "GLOBAL.h"
#include <map>
#include <string>
#include "CombinedController.h"
#include "GameShader.h"
#include "gameObject.h"
#include "Light.h"
#include "Billboard.h"
#include "UI.h"
#include "ResourceFactory.h"
#include "glTexture.h"
#include "NoiseCreator.h"
#include "Model.h"
#include "FrameBuffer.h"
glm::vec3 position = glm::vec3(0,-2,0),look = glm::vec3(0,0,0);
float speed=.1;
bool locked, warped,running=true,isgrasson;
CombinedController *CombCont;
glTexture mtext,transitionTexture;
NoiseCreator N;
FrameBuffer F;
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
        case SDLK_z:
            position.y-=1.f;
            break;
        case SDLK_p:
            {
            isgrasson=true;
            std::vector<char> pixels;
            int w=1000,h=1000;
            for(int i=0;i<w;i++)
            {
                for(int j=0;j<h;j++)
                {
                    float distanceGradient = std::min((glm::distance(glm::vec2(i,j),glm::vec2(w/2,h/2))*.0001),1.0);
                    float noiseGradient = (N.octaveNoise(8,.5,.001,glm::vec2(i,j))+1.0)/2.0;
                    float amt = std::max((noiseGradient-distanceGradient)*1.5-.5,0.0);
                    pixels.push_back(255*amt);
                    pixels.push_back(255*amt);
                    pixels.push_back(255*amt);
                }
            }
            mtext.loadFromArray(&pixels[0],w,h,3);
            mtext.setTarget(GL_TEXTURE_2D);
            mtext.setTextureProperty(GL_TEXTURE_WRAP_S,GL_REPEAT);
            mtext.setTextureProperty(GL_TEXTURE_WRAP_T,GL_REPEAT);
            mtext.setTextureProperty(GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
            mtext.setTextureProperty(GL_TEXTURE_MAG_FILTER,GL_LINEAR);
            mtext.loadTexture();
            break;
            }
        case SDLK_o:
            isgrasson=false;
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
    {
        glViewport(0,0,e.window.data1,e.window.data2);
        F.CreateFrameBuffer(e.window.data1,e.window.data2);
    }
}
int main(int argc, char *argv[])
{
    GLOBAL::Init();
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG);
    CombCont = new CombinedController(100,100,512,512,SDL_WINDOW_OPENGL| SDL_WINDOW_RESIZABLE);

    CombCont->addEvent(key,SDL_KEYDOWN);
    CombCont->addEvent(resizeViewport,SDL_WINDOWEVENT);
    CombCont->addEvent(mouseMoved,SDL_MOUSEMOTION);

    GameShader RenderShader, UIShader;
    ResourceFactory RF;
    Model *MarbleModel, *RectangleUIModel,*grassModel;
    glTexture *groundA,*groundB;
    gameObject floor,floors[3], grass;
    Light bulb;
    UI displayUI, billboardUI;
    Billboard testBillboard;
    groundB=RF.loadFromFile<glTexture>("C:/Users/Nick/Dropbox/Apps/AGE/Resources/beachsandnormal.png");
    groundA = RF.loadFromFile<glTexture>("C:/Users/Nick/Dropbox/Apps/AGE/Resources/beachsanddiffuse.png");
    MarbleModel = RF.loadFromFile<Model>("C:/Users/Nick/Dropbox/Apps/AGE/Resources/3d/GrassPlane/marbleplane.obj");
    RectangleUIModel = RF.loadFromFile<Model>("C:/Users/Nick/Dropbox/Apps/AGE/Resources/3d/plane.obj");
    grassModel = RF.loadFromFile<Model>("C:/Users/Nick/Dropbox/Apps/AGE/Resources/3d/grass/grass.obj");
    UIShader.loadFromFile("C:/Users/Nick/Dropbox/Apps/AGE/Resources/Shaders/MultiShader2D.glsl");
    RenderShader.loadFromFile("C:/Users/Nick/Dropbox/Apps/AGE/Resources/Shaders/MultiShader.glsl");
    RenderShader.LinkProgram();
    UIShader.LinkProgram();

    groundA->setTarget(GL_TEXTURE_2D);
    groundA->setTextureProperty(GL_TEXTURE_WRAP_S,GL_REPEAT);
    groundA->setTextureProperty(GL_TEXTURE_WRAP_T,GL_REPEAT);
    groundA->setTextureProperty(GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    groundA->setTextureProperty(GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    groundA->loadTexture();
    groundB->setTarget(GL_TEXTURE_2D);
    groundB->setTextureProperty(GL_TEXTURE_WRAP_S,GL_REPEAT);
    groundB->setTextureProperty(GL_TEXTURE_WRAP_T,GL_REPEAT);
    groundB->setTextureProperty(GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    groundB->setTextureProperty(GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    groundB->loadTexture();

    testBillboard.setWindow(CombCont);
    testBillboard.setShader(&RenderShader);
    testBillboard.Position = glm::normalize(glm::vec3(1,1,1));
    testBillboard.setSize(glm::vec4(1,0,1,0));
    testBillboard.setLockAxis(glm::vec3(0,1,0));
    testBillboard.setRotation(0);

    grass.setShader(&RenderShader);
    grass.setSize(glm::vec3(1,1,1));
    grass.TextureScale = glm::vec2(1,1);
    grass.setMesh(grassModel);
    floor.setWindow(CombCont);
    floor.setMesh(MarbleModel);
    floor.setSize(glm::vec3(10,1,10));
    floor.setPosition(glm::vec3(0,0,0));
    floor.TextureScale = glm::vec2(10,10);

    billboardUI.setParent(&testBillboard);
    billboardUI.setWindow(CombCont);
    billboardUI.setMesh(grassModel);
    billboardUI.setColor(glm::vec4(1,1,0,.2));
    billboardUI.setSize(glm::vec4(1,0,1,0));
    billboardUI.setPosition(glm::vec4(0,0,0,0));

    bulb.Position = glm::vec3(0,1,0);
    bulb.DiffuseColor=glm::vec4(1,1,1,1);
    bulb.SpecularColor=glm::vec4(2,2,2,1);
    bulb.AmbientColor=glm::vec4(.1,.1,.1,1);
    bulb.LinearAttenuation = .5;
    bulb.SpotCutoff = 180;
    bulb.SpotExponent = 1;
    for(int i=0;i<3;i++)
    {
        floors[i].Position = glm::vec3(0,.25,i+5);
        floors[i].Size = glm::vec3(.5,.5,0);
        switch(i)
        {
            case 0:
                floors[i].setColor(glm::vec4(0,0,1,.75));
                break;
            case 1:
                floors[i].setColor(glm::vec4(1,1,0,.75));
                break;
            case 2:
                floors[i].setColor(glm::vec4(1,0,0,.75));
                break;
        }
        floors[i].setMesh(RectangleUIModel);
    }
    displayUI.setWindow(CombCont);
    displayUI.setMesh(RectangleUIModel);
    displayUI.setColor(glm::vec4(1,1,0,.2));
    displayUI.setSize(glm::vec4(1,0,1,0));
    displayUI.setPosition(glm::vec4(0,0,0,0));
    float secondsPassed = 1;
    Uint64 t = SDL_GetPerformanceCounter();
    std::vector<Object3D*> sorted;
    int w=1;
    for(int i=0;i<w;i++)
    {

        Object3D *g = new Object3D;
        g->setSize(glm::vec3(1,1,1));
        g->setPosition(i,1,0);
        sorted.push_back(g);
    }
    while(running)
    {
        CombCont->CheckKeys();
        glDepthMask(GL_TRUE);
        RenderShader.Activate();
        glBindFragDataLocation( RenderShader.ShaderProgram, 0, "outColor" );
        glBindFragDataLocation( RenderShader.ShaderProgram, 1, "outColor1" );
        glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
        glClearColor(0.75,0.75,0.75,1);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_ALPHA_TEST);
        glDisable(GL_BLEND);
        glAlphaFunc(GL_EQUAL,1.0);
        RenderShader.setProjectionMatrix(90.0,CombCont->getAspectRatio(),.01,1000);
        RenderShader.setViewMatrix(position,look);
        RenderShader.setUniform1i("LightsOn",0);
        RenderShader.setUniform1i("TextureOn",1);
        RenderShader.setUniform3f("viewpos",-position);
        RenderShader.setUniform1i("ZeroOn",0);
        RenderShader.setUniform1i("grass",isgrasson);
        RenderShader.setUniform1i("time",SDL_GetTicks());
        RenderShader.setUniform1i("transparency",0);
        mtext.AttachAs(&RenderShader,"gtext",2);
        groundA->AttachAs(&RenderShader,"grassTexture",4);
        groundB->AttachAs(&RenderShader,"grassHeight",5);


        bulb.Render(&RenderShader);
        floor.Render(&RenderShader);



        F.Activate();
        glDepthMask(GL_FALSE);
        glDisable(GL_ALPHA_TEST);
        GLenum drawbuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
        glDrawBuffers(2, drawbuffers);
        int val[] = {0,0,0,0};
        float valf[] = {1.f,1.f,1.f,1.f};
        glClearBufferiv(GL_COLOR,0,val);
        glClearBufferfv(GL_COLOR,1,valf);
        RenderShader.setUniform1i("transparency",1);
        glEnable(GL_BLEND);
        glBlendFunci(0,GL_ONE,GL_ONE);
        glBlendFunci(1,GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);
        for(int i=0;i<sorted.size();i++)
        {
            Object3D *guy = sorted[i];
            grass.setPosition(guy->Position);
            grass.setSize(guy->Size);
            grass.Render(&RenderShader);
        }
        RenderShader.setUniform1i("TextureOn",0);
        for(int i=0;i<3;i++)
        {
            floors[i].Render(&RenderShader);
        }
        F.Deactivate();

        //--2D--
        UIShader.Activate();
        UIShader.setProjectionMatrix(glm::ortho(-CombCont->getSize().x,CombCont->getSize().x,-CombCont->getSize().y,CombCont->getSize().y));
        glDisable(GL_DEPTH_TEST);
        UIShader.setUniform4f("color",glm::vec4(1.f,1.f,.2f,1.0f));
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE_MINUS_SRC_ALPHA,GL_SRC_ALPHA);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D,F.texColorBuffer);
        glUniform1i(glGetUniformLocation(UIShader.ShaderProgram,"disptext"),2);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D,F.texColorBuffer2);
        glUniform1i(glGetUniformLocation(UIShader.ShaderProgram,"disptext2"),3);

        displayUI.Render(&UIShader);
        CombCont->Swap();


        secondsPassed = ((float)(SDL_GetPerformanceCounter()-t)/((float)SDL_GetPerformanceFrequency()));
        if(secondsPassed<1.0/300.0)
        {
            SDL_Delay(((1.0/300.0)-secondsPassed)*1000.0);
        }
        secondsPassed = ((float)(SDL_GetPerformanceCounter()-t)/((float)SDL_GetPerformanceFrequency()));
        t = SDL_GetPerformanceCounter();
        Lights.clear();
    }
    IMG_Quit();
    SDL_Quit();
    return 0;
}

