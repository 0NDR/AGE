#ifndef PHYUI_H_INCLUDED
#define PHYUI_H_INCLUDED
#include "PhysicsWorld.h"
#include "Object2D.h"
#include "Shader.h"
#include "Mesh.h"
class Physics2D: public Object2D
{
    private:
        btTransform trans;
        btQuaternion quat;
        PhysicsWorld *world;
        btDefaultMotionState *dmst;
        btCollisionShape* CollisionShape;
    public:
        btRigidBody* RigidBody;
        Physics2D(){addNewType(); RigidBody = 0;}
        Physics2D(Object* parent): Object2D(parent){addNewType();RigidBody = 0;}
        Physics2D(std::string name): Object2D(name){addNewType();RigidBody = 0;}
        Physics2D(Object* parent, std::string name): Object2D(parent,name){addNewType();RigidBody = 0;}

        float getInvMass();
        void setMass(float m,glm::vec3 inertia);
        float getFriction();
        void setFriction(float m);
        float getRollingFriction();
        void setRollingFriction(float m);
        float getRestitution();
        void setRestitution(float m);
        void setPosition(glm::vec4 newPos);

       void ConstructRigidBody(btScalar mass, glm::vec3 Intertia);
       void ConstructRigidBody(btRigidBody::btRigidBodyConstructionInfo CI);
       void SetCollisionShape(btCollisionShape* shape);
       void SetCollisionShape(Mesh* collShape, bool replaceDuplicates=false);
       void AddToWorld(PhysicsWorld *addTo);
       void removeFromWorld();
       void Update();
       bool isCollidedWith(Physics2D *Other);

       virtual int push(lua_State *l)
       {
            luabridge::push(l,this);
            return 1;
       }
       static std::string TypeID() {return "Physics2D";}
       virtual std::string type() {return "Physics2D";}
       static void RegisterLua(lua_State *l)
       {


           if(!GLOBAL::isRegistered(Object2D::TypeID(),l))
           {
               Object2D::RegisterLua(l);
           }
           GLOBAL::addRegister(TypeID(),l);
           luabridge::getGlobalNamespace(l).deriveClass<Physics2D,Object2D>(TypeID().c_str())
                                               .addConstructor<void (*)(std::string)>()
                                               .addProperty("Position",(glm::vec4 (Physics2D::*)()const)&Object2D::getPosition,&Physics2D::setPosition)
                                               .addFunction("getInvMass",(float (Physics2D::*)()const)&Physics2D::getInvMass)
                                               .addFunction("setMass",&Physics2D::setMass)
                                               .addProperty("Friction",(float (Physics2D::*)()const)&Physics2D::getFriction,&Physics2D::setFriction)
                                               .addProperty("RollingFriction",(float (Physics2D::*)()const)&Physics2D::getRollingFriction,&Physics2D::setRollingFriction)
                                               .addProperty("Restitution",(float (Physics2D::*)()const)&Physics2D::getRestitution,&Physics2D::setRestitution)
                                           .endClass();
       }
};


#endif // PHYUI_H_INCLUDED
