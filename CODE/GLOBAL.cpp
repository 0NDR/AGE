#include "GLOBAL.h"
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
std::vector<GLOBAL::ObjectCollisionPointer>Collisions;
std::vector<LuaRegistercontainer*> statereg;


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
            i--;
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







namespace GLOBAL
{
    btDiscreteDynamicsWorld *PhysicsWorld;
    btDiscreteDynamicsWorld *UIWorld;
    float pi = 3.1415926575859;
    float Framerate = 0;
    float FramerateLock = 60;
    long long frameCount = 0;
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
