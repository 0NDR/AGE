#ifndef PHYSICSOBJECT_H_INCLUDED
#define PHYSICSOBJECT_H_INCLUDED
#include "gameObject.h"

class PhysicsObject: public gameObject
{
    private:
        btTransform trans;
        btQuaternion quat;
        btDiscreteDynamicsWorld *world;
        btDefaultMotionState *dmst;
        btCollisionShape* CollisionShape;
    public:
       //static std::string TypeID(){return "PhysicsObject";}
       btRigidBody* RigidBody;
       bool isStatic;
       PhysicsObject(){addNewType(); isStatic=false;}
       PhysicsObject(Object* parent): gameObject(parent){addNewType(); isStatic=false;}
       PhysicsObject(std::string name): gameObject(name){addNewType(); isStatic=false;}
       PhysicsObject(Object* parent, std::string name): gameObject(parent,name){addNewType(); isStatic=false;}
       void Update();
       void ConstructRigidBody(btScalar mass, glm::vec3 Intertia);
       void ConstructRigidBody(btRigidBody::btRigidBodyConstructionInfo CI);
       void SetCollisionShape(btCollisionShape* shape);
       void SetCollisionShape(Mesh* collShape, bool replaceDuplicates=false);
       void AddToWorld(btDiscreteDynamicsWorld *addTo);
       void removeFromWorld();
       void setPosition(glm::vec3 Position);

       void setLinearVelocity(glm::vec3 vel);
       void setAngularVelocity(glm::vec3 vel);
       glm::vec3 getAngularVelocity() const;
       glm::vec3 getLinearVelocity() const;

       bool isCollidedWith(PhysicsObject *Other);



        static void RegisterLua(lua_State* l)
        {
            if(!GLOBAL::isRegistered(gameObject::TypeID(),l))
            {
                gameObject::RegisterLua(l);
            }
            GLOBAL::addRegister(PhysicsObject::TypeID(),l);
            luabridge::getGlobalNamespace(l).deriveClass<PhysicsObject,gameObject>(TypeID().c_str())
                                                .addConstructor<void (*)(std::string)>()
                                                .addFunction("setAngularVelocity", &PhysicsObject::setAngularVelocity)
                                                .addFunction("getAngularVelocity", &PhysicsObject::getAngularVelocity)
                                                .addFunction("setLinearVelocity", &PhysicsObject::setLinearVelocity)
                                                .addFunction("getLinearVelocity", &PhysicsObject::getLinearVelocity)
                                                .addFunction("setPosition",&PhysicsObject::setPosition)
                                            .endClass();
        }
};


#endif // PHYSICSOBJECT_H_INCLUDED
