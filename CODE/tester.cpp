#include "GLOBAL.h"
#include "Billboard.h"
#include "CombinedController.h"
#include "TextTexture.h"
#include "Font.h"
#include "GameShader.h"
#include "gameObject.h"
#include "Light.h"
#include "NoiseCreator.h"
#include "LuaScript.h"
#include "InstanceFactory.h"

#include "SoundStream.h"
#include "Speaker.h"
Object container;
Object container2D;
InstanceFactory IF;
Shader Shader2D("Shader2D");
GameShader Shader3D("Shader3D");
Mesh ManualMesh("ManMesh");
Mesh spmesh("Sphere mesh");
Mesh tMesh("Torus");
glTexture tex("Texture");
TextTexture gradient("Gradient");
Font DrawingFont("DrawingFont");
NoiseCreator n;
float speedfactorj = 10.f;
float speedfactor = 1/10.f;
bool xory = false;
float xpos = 0;
float ypos = 0;
float zoom = 0.0;
bool warped = false;
bool locked = false;
int currentSeed = 25;
glm::vec3 look,position;

int speakerindex=0;
float dist = 0;
bool clicked = false;
UI streamUI(&container2D,"controller");
gameObject WorldPlane(&container,"kek");
Speaker source(&WorldPlane,"kek");
Speaker source2(&WorldPlane,"kek2");
Sound buffer;
Sound buffer2;
CombinedController CombCont(150,100,512,512,SDL_WINDOW_OPENGL|SDL_WINDOW_BORDERLESS);

void setAudioDevice(int ind)
{
        source.Pause();
        const ALCchar* devices = alcGetString(NULL, ALC_ALL_DEVICES_SPECIFIER);
        std::string device = "";
        if( !strlen(devices) )
        {
            printf("None found.\n");
        }
        else
        {
            int index=0;
            while( devices && *devices &&index<ind )
            {
                devices += strlen(devices) + 1;
                index++;
            }
           device= devices;
        }
        std::cout<<device<<std::endl;
        CombCont.setDevice((char*)device.c_str());
        CombCont.setContext((ALCint*)NULL);
        CombCont.makeCurrent();
        ALfloat pos[] = {0,0,0};
        ALfloat vel[] = {0,0,0};
        ALfloat orie[] = {0,0,1, 0,1,0};
        ALfloat Spos[] = {0,0,1};

        alListenerfv(AL_POSITION,pos);
        alListenerfv(AL_VELOCITY,vel);
        alListenerfv(AL_ORIENTATION,orie);
        source.deleteSource();
        source2.deleteSource();
        buffer.loadFromFile("C:/Users/Nick/Dropbox/Apps/AGE/Resources/Sound/badrats.wav");
        //buffer2.loadFromFile("C:/Users/Nick/Dropbox/Apps/AGE/Resources/Sound/bass.ogg");
        source.setGain(1);
        source.setPitch(1);
        source.setRolloffFactor(.1);
        source.setPosition(glm::vec3(0,0,0));
        source.setVelocity(glm::vec3(0,0,0));
        source.setBuffer(&buffer);
        source.Play(true);

        source2.setGain(1);
        source2.setPitch(1);
        source2.setRolloffFactor(.1);
        source2.setPosition(glm::vec3(0,0,0));
        source2.setVelocity(glm::vec3(0,0,0));
        source2.setBuffer(&buffer2);
        source2.Play(true);
}
void key(SDL_Event e)
{

    switch(e.key.keysym.sym)
    {

        case SDLK_i:
            position.x-=sin(3.14159265759-(look.y/360*(2*3.14159265759)))/speedfactor*fabs(cos((look.x/360*(2*3.14159265759))));
            position.z-=cos(3.14159265759-(look.y/360*(2*3.14159265759)))/speedfactor*fabs(cos((look.x/360*(2*3.14159265759))));
            position.y+=sin((look.x/360*(2*3.14159265759)))/speedfactor;
            break;
        case SDLK_k:
            position.x+=sin(3.14159265759-(look.y/360*(2*3.14159265759)))/speedfactor*fabs(cos((look.x/360*(2*3.14159265759))));;
            position.z+=cos(3.14159265759-(look.y/360*(2*3.14159265759)))/speedfactor*fabs(cos((look.x/360*(2*3.14159265759))));;
            position.y-=sin((look.x/360*(2*3.14159265759)))/speedfactor;
            break;
        case SDLK_l:
            position.x-=cos(look.y/360*(2*3.14159265759))/speedfactor;
            position.z-=sin(look.y/360*(2*3.14159265759))/speedfactor;
            break;
        case SDLK_j:
            position.x+=cos(look.y/360*(2*3.14159265759))/speedfactor;
            position.z+=sin(look.y/360*(2*3.14159265759))/speedfactor;
            break;
        case SDLK_SPACE:
            xory=!xory;
            break;
        case SDLK_UP:
            speakerindex++;
            if(speakerindex>SDL_GetNumAudioDevices(0)*2)
            {
                speakerindex=0;
                std::cout<<"restart"<<std::endl;
            }
            setAudioDevice(0);
           break;
        case SDLK_DOWN:
            source.Pause();
            break;
        case SDLK_RIGHT:
            source.Play();
            break;
        case SDLK_p:
            DrawingFont.setPointSize(DrawingFont.getPointSize()+10);
            DrawingFont.loadFont();
            break;
        case SDLK_SEMICOLON:
            DrawingFont.setPointSize(DrawingFont.getPointSize()-10);
            DrawingFont.loadFont();
            break;
    }

}


void mouse(SDL_Event e)
{
    zoom += (float)e.wheel.y/100.f;
    SDL_Event f;
    f.type = SDL_KEYDOWN;
    SDL_PushEvent(&f);
}
void mouseDown(SDL_Event e)
{
    glm::vec2 uipos = streamUI.getPositionRelative();
    glm::vec2 mousepos = glm::vec2(e.button.x*2,e.button.y*2)/CombCont.getSize()-glm::vec2(1,1);
    std::cout<<mousepos.x<<"X"<<mousepos.y<<std::endl;
    std::cout<<uipos.x<<"X"<<uipos.y<<std::endl;
    if(abs(uipos.x-mousepos.x)<.1 && abs(uipos.y-mousepos.y)<.1)
    {
        clicked = !clicked;
        std::cout<<"click"<<std::endl;
    }
}
void mouseMoved(SDL_Event e)
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
    if (GLOBAL::Distance(e.motion.x, CombCont.getSize().x/2, e.motion.y, CombCont.getSize().y/2, 0, 0)>150 && locked)
    {
        warped = true;
        SDL_WarpMouseInWindow(CombCont.getWindow(),CombCont.getSize().x/2, CombCont.getSize().y/2);
    }
    if(clicked)
    {
//        buffer.setStreamPosition(((double)e.motion.x/(double)CombCont.getSize().x)*buffer.getStreamLength());
    }

}

void renderGoy(Object* ParentContainer,Shader* renderShader)
{
    std::vector<Object*> children = ParentContainer->getChildArray();
    for(int i=0;i<children.size();i++)
    {
        if(children[i]->isType(Renderable::TypeID()))
        {

                Renderable *ind = ((Renderable*)children[i]);
                ind->Update();
                ind->setWindow(&CombCont);
                ind->Render(renderShader);
        }
        renderGoy(children[i],renderShader);
    }
}
int main(int argc, char *argv[])
{
    GLOBAL::Init();
    SDL_Init(SDL_INIT_GAMECONTROLLER|SDL_INIT_AUDIO);
    TTF_Init();
    Mix_Init(MIX_INIT_OGG|MIX_INIT_FLAC);

    CombCont.addEvent(key,SDL_KEYDOWN);
    CombCont.addEvent(mouse,SDL_MOUSEWHEEL);
    CombCont.addEvent(mouseDown,SDL_MOUSEBUTTONDOWN);
    CombCont.addEvent(mouseMoved,SDL_MOUSEMOTION);
    SDL_GameController *controller = NULL;
    LuaScript l("kek");
    l.loadFile("C:/Users/Nick/Dropbox/Apps/AGE/Resources/Scripts/LuaTest.lua");
    std::vector<GLOBAL::Vertex> verts;
    std::vector<unsigned int> inds;

    verts.push_back(GLOBAL::Vertex(1,1,0,   0,0,0,  1,0));
    verts.push_back(GLOBAL::Vertex(1,-1,0,  0,0,0, 1,1));
    verts.push_back(GLOBAL::Vertex(-1,-1,0, 0,0,0, 0,1));
    verts.push_back(GLOBAL::Vertex(-1,1,0,  0,0,0,  0,0));

    inds.push_back(0);
    inds.push_back(1);
    inds.push_back(2);

    inds.push_back(3);
    inds.push_back(0);
    inds.push_back(2);
    RANDOM_SEEDT();

    tex.setTextureUnit(GL_TEXTURE0);
    tex.setTarget(GL_TEXTURE_2D);
    tex.setTextureProperty(GL_TEXTURE_WRAP_S,GL_REPEAT);
    tex.setTextureProperty(GL_TEXTURE_WRAP_T,GL_REPEAT);
    tex.setTextureProperty(GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    tex.setTextureProperty(GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    DrawingFont.setPath("C:/Users/Nick/Dropbox/Apps/AGE/Resources/Font/comic.ttf");
    DrawingFont.setPointSize(12);
    DrawingFont.loadFont();
    gradient.setTextureUnit(GL_TEXTURE0);
    gradient.setTarget(GL_TEXTURE_2D);
    gradient.setTextureProperty(GL_TEXTURE_WRAP_S,GL_REPEAT);
    gradient.setTextureProperty(GL_TEXTURE_WRAP_T,GL_REPEAT);
    gradient.setTextureProperty(GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    gradient.setTextureProperty(GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    gradient.loadRawFromFile("C:/Users/Nick/Dropbox/Apps/AGE/Resources/white.png");
    gradient.setBackgroundColor(glm::vec4(0,0,0,0));
    gradient.setTextColor(glm::vec4(1,0,0,.5));
    gradient.setRenderingType(TTF_RENDER_SHADED);
    gradient.setText("");
    gradient.setFont(&DrawingFont);
    gradient.loadText(true);

    glTexture normalTexture(&container,"normal");
    normalTexture.setTextureUnit(GL_TEXTURE1);
    normalTexture.setTarget(GL_TEXTURE_2D);
    normalTexture.setTextureProperty(GL_TEXTURE_WRAP_S,GL_REPEAT);
    normalTexture.setTextureProperty(GL_TEXTURE_WRAP_T,GL_REPEAT);
    normalTexture.setTextureProperty(GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    normalTexture.setTextureProperty(GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    normalTexture.loadRawFromFile("C:/Users/Nick/Dropbox/Apps/AGE/Resources/CraggyNormals2.png");
    normalTexture.loadTexture();

    ManualMesh.meshFromVector(verts,inds);
    spmesh.meshFromFile("C:/Users/Nick/Dropbox/Apps/AGE/Resources/3d/cube.obj");
    tMesh.meshFromFile("C:/Users/Nick/Dropbox/Apps/AGE/Resources/3d/plane.obj");
    Shader2D.LoadAndCompileShader(GLOBAL::textFileRead("C:/Users/Nick/Dropbox/Apps/AGE/Resources/Shaders/Shader2D.fs"),GL_FRAGMENT_SHADER);
    Shader2D.LoadAndCompileShader(GLOBAL::textFileRead("C:/Users/Nick/Dropbox/Apps/AGE/Resources/Shaders/Shader2D.vs"),GL_VERTEX_SHADER);
    Shader2D.CompileAndLinkProgram();
    Shader2D.Activate();

    Shader3D.LoadAndCompileShader(GLOBAL::textFileRead("C:/Users/Nick/Dropbox/Apps/AGE/Resources/Shaders/Shader3D.fs"),GL_FRAGMENT_SHADER);
    Shader3D.LoadAndCompileShader(GLOBAL::textFileRead("C:/Users/Nick/Dropbox/Apps/AGE/Resources/Shaders/Shader3D.vs"),GL_VERTEX_SHADER);
    Shader3D.CompileAndLinkProgram();
    Shader3D.Activate();

    WorldPlane.Color = glm::vec4(1,1,1,1);
    WorldPlane.Scale = glm::vec3(5,5,5);
    WorldPlane.Position = glm::vec3(0,0,0);
    WorldPlane.setMesh(&spmesh);

    streamUI.setPosition(glm::vec4(-1,0,0,0));
    streamUI.setColor(glm::vec4(1,1,1,1));
    streamUI.setSize(glm::vec4(.1,0,.1,0));
    streamUI.setMesh(&ManualMesh);


    Light WorldLight("Light");
    WorldLight.DiffuseColor = glm::vec4(1,1,1,1);
    WorldLight.SpecularColor = glm::vec4(1,1,1,1);
    WorldLight.AmbientColor = glm::vec4(0.1,0.1,0.1,1);
    WorldLight.LinearAttenuation = 1;
    WorldLight.Position = -position;

    GLOBAL::RegisterLua(l.getState());
    gameObject::RegisterLua(l.getState());
    Mesh::RegisterLua(l.getState());
    LuaScript::RegisterLua(l.getState());
    InstanceFactory::RegisterLua(l.getState());
    Billboard::RegisterLua(l.getState());
    Shader::RegisterLua(l.getState());
    TextTexture::RegisterLua(l.getState());
    KeyController::RegisterLua(l.getState());
    KeyController *CombContP = &CombCont;
    TextTexture* gradientp = &gradient;
    Object *containerp = &container2D;
    Mesh *spmeshp = &ManualMesh;
    Shader* s2dp = &Shader2D;
    InstanceFactory *IF2 = &IF;
    luabridge::getGlobalNamespace(l.getState()).beginNamespace("K")
                                                   .addVariable("IF",&IF2)
                                                   .addVariable("shader2",&s2dp)
                                                   .addVariable("smesh",&spmeshp)
                                                   .addVariable("texture",&gradientp)
                                                   .addVariable("Controller",&CombContP)
                                                .endNamespace();
    luabridge::setGlobal(l.getState(),&container,"game");
    luabridge::setGlobal(l.getState(),&container2D,"game2d");
    std::cout<<l.Run(true);
    while(true)
    {
        GLOBAL::frameCount++;
        CombCont.CheckKeys();
        Lights.clear();
        GLOBAL::PhysicsWorld->stepSimulation(1.0/60.0f,0);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glClearColor(0,1,1,1);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set the blend function
        glEnable(GL_DEPTH_TEST);

        Shader3D.setProjectionMatrix(75,CombCont.getSize().x/CombCont.getSize().y,.01,2000);
        Shader3D.setViewMatrix(position,glm::vec3(look.x,look.y,0));


        ALfloat campos[] = {position.x,position.y,position.z};
        glm::vec3 lookAt = glm::vec3(-Shader3D.getViewMatrix()[1][0],-Shader3D.getViewMatrix()[1][1],Shader3D.getViewMatrix()[1][2]);
        glm::vec3 up = glm::vec3(-Shader3D.getViewMatrix()[2][0],-Shader3D.getViewMatrix()[2][1],Shader3D.getViewMatrix()[2][2]);
        ALfloat camorie[] = {lookAt.x,lookAt.y,lookAt.z, up.x,up.y,up.z};
        alListenerfv(AL_POSITION,campos);
        alListenerfv(AL_ORIENTATION,camorie);

        WorldPlane.setPosition(glm::vec3(dist*cosf((float)GLOBAL::frameCount/30),0,dist*sinf((float)GLOBAL::frameCount/30)));
        float ax,ay,az;
        alGetSource3f(source.getSource(),AL_POSITION,&ax,&ay,&az);
        glUniform1i(glGetUniformLocation(Shader3D.ShaderProgram,"NormalMap"),0);
        glUniform1i(glGetUniformLocation(Shader3D.ShaderProgram,"ColorOn"),1);
        glUniform1i(glGetUniformLocation(Shader3D.ShaderProgram,"TextureOn"),1);
        glUniform1i(glGetUniformLocation(Shader3D.ShaderProgram,"LightsOn"),1);
        glUniform1i(glGetUniformLocation(Shader3D.ShaderProgram,"ZeroOn"),0);
        renderGoy(&container,&Shader3D);

        Shader2D.Activate();
        glClearColor(0,1,1,1);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        gradient.Activate();
        gradient.bindTexture();
        glUniform1i(glGetUniformLocation(Shader3D.ShaderProgram, "ObjectTexture" ),0);
/*        if(buffer.getState()&AUDIOSTATE_Loaded)
            streamUI.setPosition(glm::vec4(((buffer.getStreamPosition()/buffer.getStreamLength())*2)-1,0,0,0));*/
        renderGoy(&container2D,&Shader2D);
        CombCont.Swap();
    }
    return 0;
}

