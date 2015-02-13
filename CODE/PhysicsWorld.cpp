#include "PhysicsWorld.h"
void PhysicsWorld::init()
{
        broadphase = new btDbvtBroadphase();
        collisionConfiguration = new btDefaultCollisionConfiguration();
        dispatcher = new btCollisionDispatcher(collisionConfiguration);
//        btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher);
        solver = new btSequentialImpulseConstraintSolver;
        World = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);

        World->setGravity(btVector3(0,-60,0));
}
void PhysicsWorld::Update()
{
    World->stepSimulation(1.0/60.f,0);
}
void PhysicsWorld::setGravity(glm::vec3 grav)
{
    setGravity(grav.x,grav.y,grav.z);
}
void PhysicsWorld::setGravity(float x, float y, float z)
{
    World->setGravity(btVector3(x,y,z));
}
glm::vec3 PhysicsWorld::getGravity()
{
    btVector3 grav = World->getGravity();
    return glm::vec3(grav.getX(),grav.getY(),grav.getZ());
}
