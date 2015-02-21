#include "GLOBAL.h"
#include "Billboard.h"
#include "CombinedController.h"
#include "TextTexture.h"
#include "glTexture.h"
#include "Font.h"
#include "GameShader.h"
#include "gameObject.h"
#include "Light.h"
#include "NoiseCreator.h"
#include "LuaScript.h"
#include "InstanceFactory.h"
#include "FrameBuffer.h"
#include "SoundStream.h"
#include "Speaker.h"
#include "ResourceFactory.h"
class DualTexture: public TextTexture, public glTexture{
public:
    DualTexture(){;}
    DualTexture(Object* parent):TextTexture(parent){;}
    DualTexture(std::string name):TextTexture(name), glTexture(name){;}
    DualTexture(std::string name, Object* parent):TextTexture(parent,name){;}

    void loadText(bool drawOver=true){TextTexture::loadText(drawOver); loadTexture();}
    static std::string TypeID() {return "DualTexture";}
    virtual std::string type() {return "DualTexture";}
    static void RegisterLua(lua_State *l)
    {
        if(!GLOBAL::isRegistered(TextureBase::TypeID(),l))
        {
            TextureBase::RegisterLua(l);
        }
        if(!GLOBAL::isRegistered(Font::TypeID(),l))
        {
            Font::RegisterLua(l);
        }
        if(!GLOBAL::isRegistered(glTexture::TypeID(),l))
        {
            glTexture::RegisterLua(l);
        }
        if(!GLOBAL::isRegistered(TextTexture::TypeID(),l))
        {
            TextTexture::RegisterLua(l);
        }
        GLOBAL::addRegister(DualTexture::TypeID(),l);
        luabridge::getGlobalNamespace(l).deriveClass<DualTexture,TextTexture>(TypeID().c_str())
                                                .addConstructor<void (*)(std::string)>()
                                                .addFunction("bindTexture",&DualTexture::bindTexture)
                                                .addFunction("Activate",&DualTexture::Activate)
                                                .addFunction("loadTexture",&DualTexture::loadTexture)
                                                .addFunction("setTextureProperty",&DualTexture::setTextureProperty)
                                                .addFunction("setUniformLocation",&DualTexture::setUniformLocation)
                                                .addFunction("setTarget",&DualTexture::setTarget)
                                                .addFunction("getText",&DualTexture::getText)
                                                .addFunction("getTextColor",&DualTexture::getTextColor)
                                                .addFunction("getRenderingType",&DualTexture::getRenderingType)
                                                .addFunction("getBackgroundColor",&DualTexture::getBackgroundColor)
                                                .addFunction("getFont",&DualTexture::getFont)

                                                .addFunction("setText",&DualTexture::setText)
                                                .addFunction("setTextColor",&DualTexture::setTextColor)
                                                .addFunction("setRenderingType",&DualTexture::setRenderingType)
                                                .addFunction("setBackgroundColor",&DualTexture::setBackgroundColor)
                                                .addFunction("setFont",&DualTexture::setFont)

                                                .addFunction("clearText",&DualTexture::clearText)
                                                .addFunction("drawText",&DualTexture::drawText)
                                                .addFunction("loadText",&DualTexture::loadText)
                                                .addFunction("getTextLength",&DualTexture::getTextLength)
                                            .endClass();
            luabridge::setGlobal(l,TTF_RENDER_BLENDED,"TTF_RENDER_BLENDED");
            luabridge::setGlobal(l,TTF_RENDER_SHADED,"TTF_RENDER_SHADED");
            luabridge::setGlobal(l,TTF_RENDER_SOLID,"TTF_RENDER_SOLID");
            luabridge::setGlobal(l,GL_TEXTURE,"GL_TEXTURE");
            luabridge::setGlobal(l,GL_LINEAR,"GL_LINEAR");
            luabridge::setGlobal(l,GL_NEAREST,"GL_NEAREST");
            luabridge::setGlobal(l,GL_LINEAR_MIPMAP_LINEAR,"GL_LINEAR_MIPMAP_LINEAR");
            luabridge::setGlobal(l,GL_NEAREST_MIPMAP_LINEAR,"GL_NEAREST_MIPMAP_LINEAR");
            luabridge::setGlobal(l,GL_LINEAR_MIPMAP_NEAREST,"GL_LINEAR_MIPMAP_NEAREST");
            luabridge::setGlobal(l,GL_NEAREST_MIPMAP_NEAREST,"GL_NEAREST_MIPMAP_NEAREST");
            luabridge::setGlobal(l,GL_CLAMP,"GL_CLAMP");
            luabridge::setGlobal(l,GL_REPEAT,"GL_REPEAT");

            luabridge::setGlobal(l,GL_TEXTURE_MAG_FILTER,"GL_TEXTURE_MAG_FILTER");
            luabridge::setGlobal(l,GL_NEAREST_MIPMAP_LINEAR,"GL_TEXTURE_MIN_FILTER");
            luabridge::setGlobal(l,GL_TEXTURE_WRAP_S,"GL_TEXTURE_WRAP_S");
            luabridge::setGlobal(l,GL_TEXTURE_WRAP_T,"GL_TEXTURE_WRAP_T");
            luabridge::setGlobal(l,GL_TEXTURE_1D,"GL_TEXTURE_1D");
            luabridge::setGlobal(l,GL_TEXTURE_2D,"GL_TEXTURE_2D");
            luabridge::setGlobal(l,GL_TEXTURE_3D,"GL_TEXTURE_3D");
            luabridge::setGlobal(l,GL_TEXTURE_CUBE_MAP,"GL_TEXTURE_CUBE_MAP");
   }
};

Object container;
Object container2D;
InstanceFactory IF;
ResourceFactory RF;
Shader Shader2D("Shader2D");
GameShader Shader3D("Shader3D");
Mesh ManualMesh("ManMesh");
Model* spmesh;
Model* tMesh;
glTexture tex("Texture");
glTexture gradient("Gradient");
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
bool playing = true;
int currentSeed = 25;
glm::vec3 look,position;

int speakerindex=0;
float dist = 10;
bool clicked = false;
bool mpressed = false;
UI streamUI(&container2D,"controller");
gameObject WorldPlane("kek");
Speaker source(&WorldPlane,"kek");
Speaker source2(&WorldPlane,"kek2");
Sound buffer;
Sound buffer2;
CombinedController CombCont(100,100,512,512,SDL_WINDOW_OPENGL|SDL_WINDOW_ALLOW_HIGHDPI|SDL_WINDOW_RESIZABLE);
LuaScript l("kek");
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
        buffer.loadFromFile("C:/Users/Nick/Dropbox/Apps/AGE/Resources/Sound/bass.ogg");
        buffer2.loadFromFile("C:/Users/Nick/Dropbox/Apps/AGE/Resources/Sound/bass.ogg");
        source.setGain(1);
        source.setPitch(1);
        source.setRolloffFactor(.1);
        source.setPosition(glm::vec3(0,0,0));
        source.setVelocity(glm::vec3(0,0,0));
        source.setBuffer(&buffer);

        source2.setGain(1);
        source2.setPitch(1);
        source2.setRolloffFactor(.1);
        source2.setPosition(glm::vec3(0,0,0));
        source2.setVelocity(glm::vec3(0,0,0));
        source2.setBuffer(&buffer2);
        //source2.Play(true);
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
            locked=!locked;
            break;
        case SDLK_UP:
            speakerindex++;
            if(speakerindex>SDL_GetNumAudioDevices(0)*2)
            {
                speakerindex=0;
            }
            setAudioDevice(0);
           break;
        case SDLK_DOWN:
            source.Pause();
            break;
        case SDLK_RIGHT:
            source.Play(false);
            break;
        case SDLK_p:
            DrawingFont.setPointSize(DrawingFont.getPointSize()+10);
            DrawingFont.loadFromFile(DrawingFont.getPath());
            break;
        case SDLK_SEMICOLON:
            DrawingFont.setPointSize(DrawingFont.getPointSize()-10);
            DrawingFont.loadFromFile(DrawingFont.getPath());
            break;
        case SDLK_ESCAPE:
            playing=false;
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
void resizeViewport(SDL_Event e)
{

    if(e.window.event==SDL_WINDOWEVENT_RESIZED)
        glViewport(0,0,e.window.data1,e.window.data2);
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
    CombCont.addEvent(resizeViewport,SDL_WINDOWEVENT);
    l.loadFromFile("C:/Users/Nick/Dropbox/Apps/AGE/Resources/Scripts/LuaTest.lua");
    std::vector<Vertex> verts;
    std::vector<unsigned int> inds;

    verts.push_back(Vertex(1,1,0,   0,0,-1,  1,1));
    verts.push_back(Vertex(1,-1,0,  0,0,-1, 1,0));
    verts.push_back(Vertex(-1,-1,0, 0,0,-1, 0,0));
    verts.push_back(Vertex(-1,1,0,  0,0,-1,  0,1));

    inds.push_back(0);
    inds.push_back(1);
    inds.push_back(2);

    inds.push_back(3);
    inds.push_back(0);
    inds.push_back(2);
    RANDOM_SEEDT();

    DrawingFont.setPointSize(30);
    DrawingFont.loadFromFile("C:/Users/Nick/Dropbox/Apps/AGE/Resources/Font/comic.ttf");
    TextureBase Textures[6];
    std::string directions[] = {"RIGHT","LEFT","UP","DOWN","BACK","FORWARD"};

    gradient.setUniformLocation(1,"CubeTexture");
    gradient.setTarget(GL_TEXTURE_CUBE_MAP);
    gradient.setTextureProperty(GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
    gradient.setTextureProperty(GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    gradient.setTextureProperty(GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    gradient.setTextureProperty(GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    gradient.setTextureProperty(GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    gradient.Activate();
    gradient.bindTexture();
    for(int i=0;i<6;i++)
    {
        Textures[i].loadFromFile("C:/Users/Nick/Dropbox/Apps/AGE/Resources/skyboxa/"+directions[i]+".jpg");
        gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,Textures[i].getFormat(),Textures[i].getSurface()->w,Textures[i].getSurface()->h,Textures[i].getFormat(),GL_UNSIGNED_BYTE, Textures[i].getSurface()->pixels);
    }


    glTexture normalTexture(&container,"normal");
    normalTexture.setUniformLocation(0,"ObjectTexture");
    normalTexture.setTarget(GL_TEXTURE_2D);
    normalTexture.setTextureProperty(GL_TEXTURE_WRAP_S,GL_REPEAT);
    normalTexture.setTextureProperty(GL_TEXTURE_WRAP_T,GL_REPEAT);
    normalTexture.setTextureProperty(GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    normalTexture.setTextureProperty(GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    normalTexture.loadFromFile("C:/Users/Nick/Dropbox/Apps/AGE/Resources/Craggy.png");
    normalTexture.loadTexture();

    glTexture ntext(&container,"nmap");
    ntext.setUniformLocation(3,"Texture_Normal0");
    ntext.setTarget(GL_TEXTURE_2D);
    ntext.setTextureProperty(GL_TEXTURE_WRAP_S,GL_REPEAT);
    ntext.setTextureProperty(GL_TEXTURE_WRAP_T,GL_REPEAT);
    ntext.setTextureProperty(GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    ntext.setTextureProperty(GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
    ntext.loadFromFile("C:/Users/Nick/Dropbox/Apps/AGE/Resources/out.png");
    ntext.loadTexture();

    ManualMesh.meshFromVector(verts,inds);
    spmesh = RF.loadFromFile<Model>("C:/Users/Nick/Dropbox/Apps/AGE/Resources/3d/cube.obj");
    tMesh =  RF.loadFromFile<Model>("C:/Users/Nick/Dropbox/Apps/AGE/Resources/3d/nanosuit/nanosuit.obj");
    Shader2D.LoadAndCompileShader(GLOBAL::textFileRead("C:/Users/Nick/Dropbox/Apps/AGE/Resources/Shaders/Shader2D.fs"),GL_FRAGMENT_SHADER);
    Shader2D.LoadAndCompileShader(GLOBAL::textFileRead("C:/Users/Nick/Dropbox/Apps/AGE/Resources/Shaders/Shader2D.vs"),GL_VERTEX_SHADER);
    Shader2D.LinkProgram();
    Shader2D.Activate();

    Shader3D.LoadAndCompileShader(GLOBAL::textFileRead("C:/Users/Nick/Dropbox/Apps/AGE/Resources/Shaders/Shader3D.fs"),GL_FRAGMENT_SHADER);
    Shader3D.LoadAndCompileShader(GLOBAL::textFileRead("C:/Users/Nick/Dropbox/Apps/AGE/Resources/Shaders/Shader3D.vs"),GL_VERTEX_SHADER);
    Shader3D.LinkProgram();
    Shader3D.Activate();


    WorldPlane.Color = glm::vec4(1,1,1,1);
    WorldPlane.Scale = glm::vec3(1,1,1);
    WorldPlane.Position = glm::vec3(0,0,0);
    WorldPlane.setMesh(spmesh);

    Light WorldLight("Light");
    WorldLight.DiffuseColor = glm::vec4(0.1,0.1,0.1,1);
    WorldLight.SpecularColor = glm::vec4(0.5,0.5,0.5,1);
    WorldLight.AmbientColor = glm::vec4(0.1,0.1,0.1,1);
    WorldLight.LinearAttenuation = .01;
    WorldLight.SpotCutoff = 180;
    WorldLight.Position = glm::vec3(-.5477*10,.5328*10,-.8366*10);

    gameObject::RegisterLua(l.getState());
    Model::RegisterLua(l.getState());
    LuaScript::RegisterLua(l.getState());
    InstanceFactory::RegisterLua(l.getState());
    Billboard::RegisterLua(l.getState());
    Shader::RegisterLua(l.getState());
    TextTexture::RegisterLua(l.getState());
    KeyController::RegisterLua(l.getState());
    DualTexture::RegisterLua(l.getState());
    ResourceFactory::RegisterLua(l.getState());


    KeyController *CombContP = &CombCont;
    glTexture* gradientp = &gradient;
    Object *containerp = &container2D;
    Mesh *spmeshp = &ManualMesh;
    Shader* s2dp = &Shader2D;
    InstanceFactory *IF2 = &IF;
    ResourceFactory *RF2 = &RF;
    luabridge::getGlobalNamespace(l.getState()).beginNamespace("K")
                                                   .addVariable("IF",&IF2)
                                                   .addVariable("RF",&RF2)
                                                   .addVariable("shader2",&s2dp)
                                                   .addVariable("smesh",&spmeshp)
                                                   .addVariable("texture",&gradientp)
                                                   .addVariable("Controller",&CombContP)
                                                .endNamespace();
    luabridge::setGlobal(l.getState(),&container,"game");
    luabridge::setGlobal(l.getState(),&container2D,"game2d");
    std::cout<<l.Run(true);
    while(playing)
    {
        Uint64 TimeCurrent = SDL_GetPerformanceCounter();
        GLOBAL::frameCount++;
        CombCont.CheckKeys();
        Lights.clear();
        glClearColor(0,1,1,1);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Set the blend function
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        Shader3D.setProjectionMatrix(75,CombCont.getSize().x/CombCont.getSize().y,.01,2000);
        Shader3D.setViewMatrix(position,glm::vec3(look.x,look.y,0));


        ALfloat campos[] = {position.x,position.y,position.z};
        glm::vec3 lookAt = glm::vec3(-Shader3D.getViewMatrix()[1][0],-Shader3D.getViewMatrix()[1][1],Shader3D.getViewMatrix()[1][2]);
        glm::vec3 up = glm::vec3(-Shader3D.getViewMatrix()[2][0],-Shader3D.getViewMatrix()[2][1],Shader3D.getViewMatrix()[2][2]);
        ALfloat camorie[] = {lookAt.x,lookAt.y,lookAt.z, up.x,up.y,up.z};
        alListenerfv(AL_POSITION,campos);
        alListenerfv(AL_ORIENTATION,camorie);
        WorldLight.setPosition(-position);
        WorldPlane.setPosition(-position);
        float ax,ay,az;
        alGetSource3f(source.getSource(),AL_POSITION,&ax,&ay,&az);
        glUniform3f(glGetUniformLocation(Shader3D.ShaderProgram, "viewpos"),position.x,position.y,position.z);
        glUniform1i(glGetUniformLocation(Shader3D.ShaderProgram,"ColorOn"),1);
        glUniform1i(glGetUniformLocation(Shader3D.ShaderProgram,"TextureOn"),1);
        glUniform1i(glGetUniformLocation(Shader3D.ShaderProgram,"LightsOn"),0);
        glUniform1i(glGetUniformLocation(Shader3D.ShaderProgram,"ZeroOn"),0);
        glUniform1i(glGetUniformLocation(Shader3D.ShaderProgram, "isCube" ),1);
        ntext.Activate();
        ntext.bindTexture();
        ntext.AttachToShader(&Shader3D);

        gradient.Activate();
        gradient.bindTexture();
        gradient.AttachToShader(&Shader3D);
        glDisable(GL_DEPTH_TEST);
        WorldPlane.Render(&Shader3D);

        glEnable(GL_DEPTH_TEST);
        glUniform1i(glGetUniformLocation(Shader3D.ShaderProgram, "isCube" ),0);
        normalTexture.Activate();
        normalTexture.bindTexture();
        normalTexture.AttachToShader(&Shader3D);
        glUniform1i(glGetUniformLocation(Shader3D.ShaderProgram,"LightsOn"),1);
        WorldLight.Render(&Shader3D);
        renderGoy(&container,&Shader3D);

        CombCont.Swap();
        //std::cout<<"FPS: "<<SDL_GetPerformanceFrequency()/(SDL_GetPerformanceCounter()-TimeCurrent)<<std::endl;
    }
    return 0;
}

