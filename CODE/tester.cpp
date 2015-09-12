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
#include "InstanceFactory.h"
#include "glTexture.h"
#include "NoiseCreator.h"
#include "Model.h"
#include "FrameBuffer.h"
#include "LuaScript.h"
#include "LuaWorld.h"
glm::vec3 position = glm::vec3(0,-2,0),look = glm::vec3(0,0,0);
float speed=.1;
bool locked, warped,running=true,isgrasson;
CombinedController *CombCont;
glTexture mtext,transitionTexture;
NoiseCreator N;
FrameBuffer Ftransparent,Fopaque;
GameShader RenderShader, UIShader;

void key(SDL_Event e)
{

    switch(e.key.keysym.sym)
    {
        case SDLK_r:
            glUseProgram(0);
            UIShader.deleteShaderProgram();
            UIShader.loadFromFile("C:/Users/Nick/Dropbox/Apps/AGE/Resources/Shaders/MultiShader2D.glsl");
            UIShader.LinkProgram();
            RenderShader.deleteShaderProgram();
            RenderShader.loadFromFile("C:/Users/Nick/Dropbox/Apps/AGE/Resources/Shaders/MultiShader.glsl");
            RenderShader.LinkProgram();
            break;
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
        if (GLOBAL::Distance(e.motion.x, CombCont->getSize().x/2, e.motion.y, CombCont->getSize().y/2, 0, 0)>2)
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
        Ftransparent.setResolution(e.window.data1,e.window.data2);
        Fopaque.setResolution(e.window.data1,e.window.data2);
    }
}
int main(int argc, char *argv[])
{
    GLOBAL::Init();
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG);
    CombCont = new CombinedController(100,100,512,512,SDL_WINDOW_OPENGL| SDL_WINDOW_RESIZABLE);
    Ftransparent.CreateFrameBuffer(512,512);
    Fopaque.CreateFrameBuffer(512,512);

    CombCont->addEvent(key,SDL_KEYDOWN);
    CombCont->addEvent(resizeViewport,SDL_WINDOWEVENT);
    CombCont->addEvent(mouseMoved,SDL_MOUSEMOTION);

    ResourceFactory RF;
    InstanceFactory IF;
    LuaWorld lWorld;
    LuaScript *lscript = new LuaScript();
    Model *MarbleModel, *RectangleUIModel,*grassModel;
    glTexture *groundA,*groundB,*Smoke;
    gameObject floor,floors[36], grass;
    Object BASE;
    Light bulb;
    UI displayUI, billboardUI;
    Billboard testBillboard;
    gameObject::RegisterLua(lWorld.getState());
    LuaScript::RegisterLua(lWorld.getState());
    InstanceFactory::RegisterLua(lWorld.getState());
    lWorld.attachScript(lscript);
    lscript->loadFromFile("C:/Users/Nick/Dropbox/Apps/AGE/Resources/Scripts/LuaTest.lua");
    Smoke=RF.loadFromFile<glTexture>("C:/Users/Nick/Dropbox/Apps/AGE/Resources/firesheet3.png");
    groundB=RF.loadFromFile<glTexture>("C:/Users/Nick/Dropbox/Apps/AGE/Resources/beachsandnormal.png");
    groundA = RF.loadFromFile<glTexture>("C:/Users/Nick/Dropbox/Apps/AGE/Resources/beachsanddiffuse.png");
    MarbleModel = RF.loadFromFile<Model>("C:/Users/Nick/Dropbox/Apps/AGE/Resources/3d/GrassPlane/marbleplane.obj");
    RectangleUIModel = RF.loadFromFile<Model>("C:/Users/Nick/Dropbox/Apps/AGE/Resources/3d/plane.obj");
    grassModel = RF.loadFromFile<Model>("C:/Users/Nick/Dropbox/Apps/AGE/Resources/3d/grass/grass.obj");
    UIShader.loadFromFile("C:/Users/Nick/Dropbox/Apps/AGE/Resources/Shaders/MultiShader2D.glsl");
    RenderShader.loadFromFile("C:/Users/Nick/Dropbox/Apps/AGE/Resources/Shaders/MultiShader.glsl");
    RenderShader.LinkProgram();
    UIShader.LinkProgram();

    Smoke->setTarget(GL_TEXTURE_2D);
    Smoke->setTextureProperty(GL_TEXTURE_WRAP_S,GL_REPEAT);
    Smoke->setTextureProperty(GL_TEXTURE_WRAP_T,GL_REPEAT);
    Smoke->setTextureProperty(GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    Smoke->setTextureProperty(GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    Smoke->loadTexture();
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

    BASE.setName("Ahh");

    grass.setShader(&RenderShader);
    grass.setSize(glm::vec3(1,1,1));
    grass.TextureScale = glm::vec2(1,1);
    grass.setMesh(grassModel);
    floor.setWindow(CombCont);
    floor.setMesh(MarbleModel);
    floor.setSize(glm::vec3(10,1,10));
    floor.setPosition(glm::vec3(0,0,0));
    floor.TextureScale = glm::vec2(10,10);

    bulb.Position = glm::vec3(0,1,0);
    bulb.Direction = glm::vec3(0,0,1);
    bulb.DiffuseColor=glm::vec4(2,2,2,1);
    bulb.SpecularColor=glm::vec4(5,5,5,1);
    bulb.AmbientColor=glm::vec4(.01,.01,.01,1);
    bulb.LinearAttenuation = .5;
    bulb.SpotCutoff = 45;
    bulb.SpotExponent = 10;
    for(int i=0;i<36;i++)
    {
        floors[i].setParent(&BASE);
        floors[i].Position = glm::vec3(i/6,1,i%6);
        floors[i].Size = glm::vec3(1,1,1);
        floors[i].Rotation = glm::vec3(0,RANDOM_INT(0,3600)/10,0);
        floors[i].setMesh(grassModel);
        floors[i].TextureScale = glm::vec2(1.0,1.0);
        floors[i].setTexture(Smoke);
    }
    displayUI.setWindow(CombCont);
    displayUI.setMesh(RectangleUIModel);
    displayUI.setColor(glm::vec4(1,1,0,.2));
    displayUI.setSize(glm::vec4(1,0,1,0));
    displayUI.setPosition(glm::vec4(0,0,0,0));
    float secondsPassed = 1;
    Uint64 t = SDL_GetPerformanceCounter();



    luabridge::setGlobal(lWorld.getState(),&BASE,"Floor");
    luabridge::setGlobal(lWorld.getState(),&IF,"Instance");

    IF.setDefaultObject(&floors[0]);
    lWorld.startScripts();
    while(running)
    {
        t = SDL_GetPerformanceCounter();
        lWorld.continueScripts();
        CombCont->CheckKeys();
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
        Fopaque.Activate();
        RenderShader.Activate();
        glBindFragDataLocation( RenderShader.ShaderProgram, 0, "outColor" );
        glBindFragDataLocation( RenderShader.ShaderProgram, 1, "outColor1" );
        glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
        glClearColor(0,0,0,1);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GEQUAL,.9);
        RenderShader.setProjectionMatrix(90.0,CombCont->getAspectRatio(),.01,1000);
        RenderShader.setViewMatrix(position,look);
        RenderShader.setUniform1i("LightsOn",1);
        RenderShader.setUniform1i("NormalsOn",1);
        RenderShader.setUniform1i("TextureOn",1);
        RenderShader.setUniform3f("viewpos",-position);
        RenderShader.setUniform1i("ZeroOn",0);
        RenderShader.setUniform1i("grass",isgrasson);
        RenderShader.setUniform1i("time",SDL_GetTicks());
        RenderShader.setUniform1i("transparency",0);
        mtext.AttachAs(&RenderShader,"gtext",2);
        groundA->AttachAs(&RenderShader,"grassTexture",4);
        groundB->AttachAs(&RenderShader,"grassHeight",5);
        float theta = -look.y+180;
        float phi = look.x+90.f;
        theta = DegreesToRadians(theta);
        phi = DegreesToRadians(phi);
        bulb.Direction =glm::vec3(sinf(theta)*sinf(phi),cosf(phi),cosf(theta)*sinf(phi));
        glm::vec4 testf = glm::inverse(RenderShader.getViewMatrix())*glm::vec4(-.5,-.5,0,0);
        bulb.Position = -position+glm::vec3(testf.x,testf.y,testf.z);
        bulb.Render(&RenderShader);
        RenderShader.setUniform1i("normalsphere",0);
        floor.Render(&RenderShader);
        RenderShader.setUniform1i("normalsphere",1);
        for(int i=0;i<BASE.getChildArray().size();i++)
        {
            RenderShader.setUniform1i("time",SDL_GetTicks());
            ((gameObject*)BASE.getChildArray()[i])->Render(&RenderShader);
        }
        RenderShader.setUniform1i("grass",0);

        Fopaque.Deactivate();
        //glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
        //glBindFramebuffer(GL_DRAW_FRAMEBUFFER, F.frameBuffer);

        //glBlitFramebuffer(0, 0, CombCont->getSize().x, CombCont->getSize().y, 0, 0, CombCont->getSize().x, CombCont->getSize().y, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        glDepthMask(GL_FALSE);
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_ALPHA_TEST);
        Ftransparent.Activate();
        glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, Fopaque.texDepthBuffer,0);
        GLenum drawbuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
        glDrawBuffers(2, drawbuffers);
        int val[] = {0,0,0,0};
        float valf[] = {1.f,1.f,1.f,1.f};
        glClearBufferiv(GL_COLOR,0,val);
        glClearBufferfv(GL_COLOR,1,valf);
        RenderShader.setUniform1i("transparency",1);
        glBlendFunci(0,GL_ONE,GL_ONE);
        glBlendFunci(1,GL_ZERO, GL_ONE_MINUS_SRC_COLOR);

        RenderShader.setUniform1i("TextureOn",1);
        RenderShader.setUniform1i("NormalsOn",0);

        //bulb.Render(&RenderShader);
        RenderShader.setUniform1i("normalsphere",1);
        for(int i=0;i<BASE.getChildArray().size();i++)
        {
            RenderShader.setUniform1i("time",SDL_GetTicks());
            ((gameObject*)BASE.getChildArray()[i])->Render(&RenderShader);
        }
        Ftransparent.Deactivate();

        //--2D--
        UIShader.Activate();
        UIShader.setProjectionMatrix(glm::ortho(-CombCont->getSize().x,CombCont->getSize().x,-CombCont->getSize().y,CombCont->getSize().y,0.f,1000.f));
        glDisable(GL_DEPTH_TEST);
        UIShader.setUniform4f("color",glm::vec4(1.f,1.f,.2f,1.0f));
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glClearColor(0,0,0,1);
        glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D,Ftransparent.texColorBuffer);
        glUniform1i(glGetUniformLocation(UIShader.ShaderProgram,"disptext"),2);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D,Fopaque.texColorBuffer);
        glUniform1i(glGetUniformLocation(UIShader.ShaderProgram,"bkg"),4);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D,Ftransparent.texColorBuffer2);
        glUniform1i(glGetUniformLocation(UIShader.ShaderProgram,"disptext2"),3);

        glUniform1i(glGetUniformLocation(UIShader.ShaderProgram,"isbkg"),1);
        displayUI.Render(&UIShader);
        glUniform1i(glGetUniformLocation(UIShader.ShaderProgram,"isbkg"),0);
        displayUI.Render(&UIShader);
        CombCont->Swap();
        Lights.clear();
        secondsPassed = ((float)(SDL_GetPerformanceCounter()-t)/((float)SDL_GetPerformanceFrequency()));
        /*if(secondsPassed<.031)
        {
            SDL_Delay((.031-secondsPassed)*1000.0);
        }*/
        secondsPassed = ((float)(SDL_GetPerformanceCounter()-t)/((float)SDL_GetPerformanceFrequency()));
        //std::cout<<1.0/secondsPassed<<std::endl;
    }
    IMG_Quit();
    SDL_Quit();
    return 0;
}

