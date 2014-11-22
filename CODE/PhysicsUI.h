#ifndef PHYUI_H_INCLUDED
#define PHYUI_H_INCLUDED
#include "UI.h"
#include "Shader.h"
#include "Mesh.h"
class PhysicsUI: public UI
{
    private:
        btTransform trans;
        btQuaternion quat;
        btDiscreteDynamicsWorld *world;
        btDefaultMotionState *dmst;
        btCollisionShape* CollisionShape;
    public:
        btRigidBody* RigidBody;
        PhysicsUI(){addNewType(); RigidBody = 0;}
        PhysicsUI(Object* parent): UI(parent){addNewType();RigidBody = 0;}
        PhysicsUI(std::string name): UI(name){addNewType();RigidBody = 0;}
        PhysicsUI(Object* parent, std::string name): UI(parent,name){addNewType();RigidBody = 0;}

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
       void AddToWorld(btDiscreteDynamicsWorld *addTo);
       void removeFromWorld();
       void Update();
       bool isCollidedWith(PhysicsUI *Other);

       virtual int push(lua_State *l)
       {
            luabridge::push(l,this);
            return 1;
       }
       static std::string TypeID() {return "PhysicsUI";}
       virtual std::string type() {return "PhysicsUI";}
       static void RegisterLua(lua_State* l)
       {
           if(!GLOBAL::isRegistered(UI::TypeID(),l))
           {
               UI::RegisterLua(l);
           }
           GLOBAL::addRegister(TypeID(),l);
           luabridge::getGlobalNamespace(l).deriveClass<PhysicsUI,UI>(TypeID().c_str())
                                               .addConstructor<void (*)(std::string)>()
                                               .addProperty("Position",(glm::vec4 (PhysicsUI::*)()const)&UI::getPosition,&PhysicsUI::setPosition)
                                               .addFunction("getInvMass",(float (PhysicsUI::*)()const)&PhysicsUI::getInvMass)
                                               .addFunction("setMass",&PhysicsUI::setMass)
                                               .addProperty("Friction",(float (PhysicsUI::*)()const)&PhysicsUI::getFriction,&PhysicsUI::setFriction)
                                               .addProperty("RollingFriction",(float (PhysicsUI::*)()const)&PhysicsUI::getRollingFriction,&PhysicsUI::setRollingFriction)
                                               .addProperty("Restitution",(float (PhysicsUI::*)()const)&PhysicsUI::getRestitution,&PhysicsUI::setRestitution)
                                           .endClass();
       }
};


#endif // PHYUI_H_INCLUDED
