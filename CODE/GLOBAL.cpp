#include "GLOBAL.h"
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
std::vector<GLOBAL::ObjectCollisionPointer>Collisions;
std::vector<LuaRegistercontainer*> statereg;

void luaWait(int ms)
{
    Uint32 time = SDL_GetTicks();
    while((int)(SDL_GetTicks()-time)<ms){;}
}
long luaClock()
{
    return (long)SDL_GetTicks();
}
bool equalsCollision(GLOBAL::ObjectCollisionPointer* a,GLOBAL::ObjectCollisionPointer* b)
{
    return (a->FirstPointer == b->FirstPointer&&a->SecondPointer == b->SecondPointer)||(a->FirstPointer == b->SecondPointer&&a->SecondPointer == b->FirstPointer);
}

void removeCollision(GLOBAL::ObjectCollisionPointer *col)
{
        for(int i=0;i<Collisions.size();i++)
        {
        if(equalsCollision(col,&Collisions[i]))
            Collisions.erase(Collisions.begin()+i);
        }
}

bool cContactDestroyedCallback(void* userData)
{
    GLOBAL::ObjectCollisionPointer *newCollision = (GLOBAL::ObjectCollisionPointer*)userData;
    std::cout<<"Collision Destroyed"<<std::endl;
    removeCollision(newCollision);
    SDL_Event pushE;
    SDL_zero(pushE);
    pushE.type = GLOBAL::BULLET_CONTACT_DESTROYED;
    pushE.user.data1 = newCollision->FirstPointer;
    pushE.user.data2 = newCollision->SecondPointer;
    SDL_PushEvent(&pushE);
    return false;
}

bool cContactAddedCallback( btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0,int partId0,
                              int index0,const btCollisionObjectWrapper* colObj1,int partId1,int index1 ) {


    GLOBAL::ObjectCollisionPointer newCollision;
    newCollision.FirstPointer = colObj0->getCollisionObject()->getUserPointer();
    newCollision.SecondPointer = colObj1->getCollisionObject()->getUserPointer();
    cp.m_userPersistentData = &newCollision;
    //if(!GLOBAL::checkCollision(&newCollision))
    //{
        Collisions.push_back(newCollision);
        SDL_Event pushE;
        SDL_zero(pushE);
        pushE.type = GLOBAL::BULLET_CONTACT_CREATED;
        pushE.user.data1 = colObj0->getCollisionObject()->getUserPointer();
        pushE.user.data2 = colObj1->getCollisionObject()->getUserPointer();
        SDL_PushEvent(&pushE);
    //}
   return false;
}
int lastFrame;
long lastTime = SDL_GetTicks();
long frameTicks = 0;


struct vec2help: public glm::vec2
{
    glm::vec2 operator+(glm::vec2 &other)
    {
        return glm::vec2(x+other.x,y+other.y);
    }
    glm::vec2 operator-(glm::vec2 &other)
    {
        return glm::vec2(x-other.x,y-other.y);
    }
    glm::vec2 operator*(float other)
    {
        return glm::vec2(x*other,y*other);
    }
    glm::vec2 power(float other)
    {
        return glm::vec2(pow(x,other),pow(y,other));
    }
    glm::vec2 operator/(float other)
    {
        return glm::vec2(x/other,y/other);
    }
    bool operator==(glm::vec2 &other)
    {
        return x==other.x&&y==other.y;
    }
    std::string getString()
    {
        std::stringstream ss;
        ss<<x<<" x "<<y;
        return ss.str();
    }
};

struct vec3help: public glm::vec3{
    glm::vec3 operator+(glm::vec3 &other)
    {
        return glm::vec3(x+other.x,y+other.y,z+other.z);
    }
    glm::vec3 operator-(glm::vec3 &other)
    {
        return glm::vec3(x-other.x,y-other.y,z-other.z);
    }
    glm::vec3 operator*(float other)
    {
        return glm::vec3(x*other,y*other,z*other);
    }
    glm::vec3 power(float other)
    {
        return glm::vec3(pow(x,other),pow(y,other),pow(z,other));
    }
    glm::vec3 operator/(float other)
    {
        return glm::vec3(x/other,y/other,z/other);
    }
    bool operator==(glm::vec3 &other)
    {
        return x==other.x&&y==other.y&&z==other.z;
    }
    std::string getString()
    {
        std::stringstream ss;
        ss<<x<<" x "<<y<<" x "<<z;
        return ss.str();
    }
};

struct vec4help: public glm::vec4{
    glm::vec4 operator+(glm::vec4 &other)
    {
        return glm::vec4(x+other.x,y+other.y,z+other.z,w+other.w);
    }
    glm::vec4 operator-(glm::vec4 &other)
    {
        return glm::vec4(x-other.x,y-other.y,z-other.z,w-other.w);
    }
    glm::vec4 operator*(float other)
    {
        return glm::vec4(x*other,y*other,z*other,w*other);
    }
    glm::vec4 power(float other)
    {
        return glm::vec4(pow(x,other),pow(y,other),pow(z,other),pow(w,other));
    }
    glm::vec4 operator/(float other)
    {
        return glm::vec4(x/other,y/other,z/other,w/other);
    }
    bool operator==(glm::vec4 &other)
    {
        return x==other.x&&y==other.y&&z==other.z&&w==other.w;
    }
    std::string getString()
    {
        std::stringstream ss;
        ss<<x<<" x "<<y<<" x "<<z<<" x "<<w;
        return ss.str();
    }

};




namespace GLOBAL
{
    btDiscreteDynamicsWorld *PhysicsWorld;
    btDiscreteDynamicsWorld *UIWorld;
    float pi = 3.1415926575859;
    float Framerate = 0;
    float FramerateLock = 60;
    int frameCount = 0;
    Uint32 BULLET_CONTACT_CREATED;
    Uint32 BULLET_CONTACT_DESTROYED;
    float Distance(float X1, float X2, float Y1, float Y2,float Z1,float Z2){
        float x = (pow((pow((X1-X2),2)+pow((Y1-Y2),2)+pow((Z1-Z2),2)),.5));
        return x;
    }
    float Clamp(float Value, float Min, float Max){
        if (Value<Min){Value=Min;}
        else if(Value>Max){Value = Max;}
        return Value;
    }
    float LoopClamp(float Value, float Min, float Max){
        if (Value<Min){Value=Max;}
        else if(Value>Max){Value = Min;}
        return Value;
    }
    bool checkCollision(GLOBAL::ObjectCollisionPointer* col)
    {
        for(int i=0;i<Collisions.size();i++)
        {
            if(equalsCollision(col,&Collisions[i]))
                return true;
        }
        return false;
    }
    bool checkCollision(void *a, void *b)
    {
        GLOBAL::ObjectCollisionPointer newC;
        newC.FirstPointer = a;
        newC.SecondPointer = b;
        return checkCollision(&newC);
    }
    void Init()
    {
        btBroadphaseInterface* broadphase = new btDbvtBroadphase();
        btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
        btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
        btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher);
        btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
        PhysicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
        PhysicsWorld->setGravity(btVector3(0,-60,0));


        btBroadphaseInterface* UIbroadphase = new btDbvtBroadphase();
        btDefaultCollisionConfiguration* UIcollisionConfiguration = new btDefaultCollisionConfiguration();
        btCollisionDispatcher* UIdispatcher = new btCollisionDispatcher(collisionConfiguration);
        btSequentialImpulseConstraintSolver* UIsolver = new btSequentialImpulseConstraintSolver;
        UIWorld = new btDiscreteDynamicsWorld(UIdispatcher,UIbroadphase,UIsolver,UIcollisionConfiguration);
        UIWorld->setGravity(btVector3(0,-100,0));

        BULLET_CONTACT_CREATED = SDL_RegisterEvents(1);
        BULLET_CONTACT_DESTROYED = SDL_RegisterEvents(1);
        gContactAddedCallback = cContactAddedCallback;
        //gContactDestroyedCallback = cContactDestroyedCallback;
    }

    void startFrame()
    {
        if(frameCount%100==0)
        {
            lastTime = SDL_GetTicks();
            lastFrame = 0;
        }
        frameCount++;
        lastFrame++;
        frameTicks = SDL_GetTicks();

        Collisions.clear();
    }
    void endFrame()
    {
        Framerate = (float)lastFrame/(((float)SDL_GetTicks()-lastTime)/1000);
        if(FramerateLock!=0)
        {
            float desired = 1000/(float)FramerateLock;
            frameTicks = SDL_GetTicks()-frameTicks;
                if(frameTicks<desired)
                    SDL_Delay(desired - frameTicks);
        }
    }
    char *textFileRead(std::string filename)
    {
        char* fn = (char*)filename.c_str();
        FILE *fp;
        char *content = NULL;

        int count=0;

        if (fn != NULL) {

            fp = fopen(fn,"rt");

            if (fp != NULL) {

                fseek(fp, 0, SEEK_END);
                count = ftell(fp);
                rewind(fp);

                if (count > 0) {
                    content = (char *)malloc(sizeof(char) * (count+1));
                    count = fread(content,sizeof(char),count,fp);
                    content[count] = '\0';
                }
                fclose(fp);

            }
            else{
                std::cout<<filename<<" not found."<<std::endl;
                return "NULL";
            }
        }
    return content;
    }

    void RegisterLua(lua_State *l)
    {
        GLOBAL::addRegister("GLOBAL",l);
        luabridge::getGlobalNamespace(l)
                                        .addFunction("wait",luaWait)
                                        .addFunction("Clock",luaClock)
                                        .beginClass<glm::vec2>("vec2")
                                            .addConstructor<void (*)(float, float)>()
                                            .addData("x",&vec2help::x)
                                            .addData("y",&vec2help::y)
                                            .addFunction("__add",(glm::vec2  (glm::vec2 ::*) (const glm::vec2 &) const )&vec2help::operator+)
                                            .addFunction("__sub",(glm::vec2  (glm::vec2 ::*) (const glm::vec2 &) const )&vec2help::operator-)
                                            .addFunction("__mul",(glm::vec2  (glm::vec2 ::*) (const float) const )&vec2help::operator*)
                                            .addFunction("__div",(glm::vec2  (glm::vec2 ::*) (const float) const )&vec2help::operator/)
                                            .addFunction("__pow",(glm::vec2  (glm::vec2 ::*) (const float) const )&vec2help::power)
                                            .addFunction("__eq",(bool (glm::vec2 ::*) (const glm::vec2 &) const )&vec2help::operator==)
                                            .addFunction("__tostring",(std::string  (glm::vec2 ::*) () const )&vec2help::getString)
                                        .endClass()
                                        .beginClass<glm::vec3>("vec3")
                                            .addConstructor<void (*)(float, float, float)>()
                                            .addData("x",&vec3help::x)
                                            .addData("y",&vec3help::y)
                                            .addData("z",&vec3help::z)
                                            .addData("r",&vec3help::x)
                                            .addData("g",&vec3help::y)
                                            .addData("b",&vec3help::z)
                                            .addFunction("__add",(glm::vec3  (glm::vec3 ::*) (const glm::vec3 &) const )&vec3help::operator+)
                                            .addFunction("__sub",(glm::vec3  (glm::vec3 ::*) (const glm::vec3 &) const )&vec3help::operator-)
                                            .addFunction("__mul",(glm::vec3  (glm::vec3 ::*) (const float) const )&vec3help::operator*)
                                            .addFunction("__div",(glm::vec3  (glm::vec3 ::*) (const float) const )&vec3help::operator/)
                                            .addFunction("__pow",(glm::vec3  (glm::vec3 ::*) (const float) const )&vec3help::power)
                                            .addFunction("__eq",(bool (glm::vec3 ::*) (const glm::vec3 &) const )&vec3help::operator==)
                                            .addFunction("__tostring",(std::string  (glm::vec3 ::*) () const )&vec3help::getString)
                                        .endClass()
                                        .beginClass<glm::vec4>("vec4")
                                            .addConstructor<void (*)(float, float, float, float)>()
                                            .addData("x",&vec4help::x)
                                            .addData("y",&vec4help::y)
                                            .addData("z",&vec4help::z)
                                            .addData("w",&vec4help::w)
                                            .addData("r",&vec4help::x)
                                            .addData("g",&vec4help::y)
                                            .addData("b",&vec4help::z)
                                            .addData("a",&vec4help::w)
                                            .addFunction("__add",(glm::vec4  (glm::vec4 ::*) (const glm::vec4 &) const )&vec4help::operator+)
                                            .addFunction("__sub",(glm::vec4  (glm::vec4 ::*) (const glm::vec4 &) const )&vec4help::operator-)
                                            .addFunction("__mul",(glm::vec4  (glm::vec4 ::*) (const float) const )&vec4help::operator*)
                                            .addFunction("__div",(glm::vec4  (glm::vec4 ::*) (const float) const )&vec4help::operator/)
                                            .addFunction("__pow",(glm::vec4  (glm::vec4 ::*) (const float) const )&vec4help::power)
                                            .addFunction("__eq",(bool (glm::vec4 ::*) (const glm::vec4 &) const )&vec4help::operator==)
                                            .addFunction("__tostring",(std::string  (glm::vec4 ::*) () const )&vec4help::getString)
                                        .endClass();
    }
    LuaRegistercontainer* getContainer(lua_State *l)
    {
        for(int i=0;i<statereg.size();i++)
        {
            if(statereg[i]->l == l)
                return statereg[i];
        }
        LuaRegistercontainer *k = new LuaRegistercontainer;
        k->l = l;
        statereg.push_back(k);
        return k;
    }
    bool isRegistered(std::string TypeID, lua_State *l)
    {
        LuaRegistercontainer *cont = getContainer(l);
        for(int i=0;i<cont->Registers.size();i++)
        {
            if(cont->Registers[i] == TypeID)
                return true;
        }
        return false;
    }
    void addRegister(std::string TypeID, lua_State *l)
    {
        LuaRegistercontainer *cont = getContainer(l);
        cont->Registers.push_back(TypeID);
    }
}
