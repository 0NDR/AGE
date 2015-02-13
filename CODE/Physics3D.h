#ifndef PHYSICSOBJECT_H_INCLUDED
#define PHYSICSOBJECT_H_INCLUDED
#include "Object3D.h"
#include "Mesh.h"
#include "PhysicsWorld.h"
class Physics3D: public Object3D
{
    private:
        btTransform trans;
        btQuaternion quat;
        PhysicsWorld *world;
        btDefaultMotionState *dmst;
        btCollisionShape* CollisionShape;
    public:
       //static std::string TypeID(){return "Physics3D";}
       btRigidBody* RigidBody;
       bool isStatic;
       Physics3D(){addNewType(); isStatic=false;}
       Physics3D(Object* parent): Object3D(parent){addNewType(); isStatic=false;}
       Physics3D(std::string name): Object3D(name){addNewType(); isStatic=false;}
       Physics3D(Object* parent, std::string name): Object3D(parent,name){addNewType(); isStatic=false;}
       void Update();
       void ConstructRigidBody(btScalar mass, glm::vec3 Intertia);
       void ConstructRigidBody(btRigidBody::btRigidBodyConstructionInfo CI);
       void SetCollisionShape(btCollisionShape* shape);
       void SetCollisionShape(Mesh* collShape, bool replaceDuplicates=false);
       void AddToWorld(PhysicsWorld *addTo);
       void removeFromWorld();
       void setPosition(glm::vec3 Position);

       void setLinearVelocity(glm::vec3 vel);
       void setAngularVelocity(glm::vec3 vel);
       glm::vec3 getAngularVelocity() const;
       glm::vec3 getLinearVelocity() const;

       bool isCollidedWith(Physics3D *Other);



        static void RegisterLua(lua_State *l)
        {



            if(!GLOBAL::isRegistered(Object3D::TypeID(),l))
            {
                Object3D::RegisterLua(l);
            }
            GLOBAL::addRegister(Physics3D::TypeID(),l);
            luabridge::getGlobalNamespace(l).deriveClass<Physics3D,Object3D>(TypeID().c_str())
                                                .addConstructor<void (*)(std::string)>()
                                                .addFunction("setAngularVelocity", &Physics3D::setAngularVelocity)
                                                .addFunction("getAngularVelocity", &Physics3D::getAngularVelocity)
                                                .addFunction("setLinearVelocity", &Physics3D::setLinearVelocity)
                                                .addFunction("getLinearVelocity", &Physics3D::getLinearVelocity)
                                                .addFunction("setPosition",&Physics3D::setPosition)
                                            .endClass();
    } ///<Adds the class definition to a given lua state
};


#endif // PHYSICSOBJECT_H_INCLUDED
