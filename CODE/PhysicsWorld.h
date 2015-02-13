#ifndef PHYWORLD_H_INCLUDED
#define PHYWORLD_H_INCLUDED
#include "GLOBAL.h"
class PhysicsWorld
{
    private:

    public:
       btDiscreteDynamicsWorld* World;
       btBroadphaseInterface* broadphase;
       btDefaultCollisionConfiguration* collisionConfiguration;
       btSequentialImpulseConstraintSolver* solver;
       btCollisionDispatcher* dispatcher;


       void init();
       void Update();
       void setGravity(glm::vec3 grav);
       void setGravity(float x, float y, float z);
       glm::vec3 getGravity();
       bool checkCollision(GLOBAL::ObjectCollisionPointer* col);
       bool checkCollision(void *a, void *b);
       virtual int push(lua_State *l)
       {
            luabridge::push(l,this);
            return 1;
       }
       static std::string TypeID() {return "PhysicsWorld";}
       virtual std::string type() {return "PhysicsWorld";}
       static void RegisterLua(lua_State *l)
       {


           GLOBAL::addRegister(TypeID(),l);
           luabridge::getGlobalNamespace(l).beginClass<PhysicsWorld>(TypeID().c_str())
                                           .endClass();
       }
};


#endif // PHYWORLD_H_INCLUDED
