#include "Physics3D.h"
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include <BulletCollision/Gimpact/btCompoundFromGimpact.h>







void Physics3D::Update()
{
    btTransform trans;
    RigidBody->getMotionState()->getWorldTransform(trans);
    btVector3 newPos = trans.getOrigin();
    btQuaternion transQuat = trans.getRotation();
    glm::quat quater = glm::quat(-transQuat.getZ(),transQuat.getY(),-transQuat.getX(),transQuat.getW());
    glm::vec3 rot = glm::eulerAngles(quater);
    Rotation = rot;
    Position = glm::vec3(newPos.getX(),newPos.getY(),newPos.getZ());
}
void Physics3D::setPosition(glm::vec3 pos)
{
    glm::vec3 transAmount = pos-Position;
    if(RigidBody)
        RigidBody->translate(btVector3(transAmount.x,transAmount.y,transAmount.z));
    Position = pos;
}
void Physics3D::SetCollisionShape(btCollisionShape* col)
{
    CollisionShape = col;
}
void Physics3D::SetCollisionShape(Mesh* collShape, bool replaceDuplicates)
{
     int numTriangles = collShape->Indices.size()/3;
     int numVerticies = collShape->VertexData.size();
     btScalar *vert = new btScalar[numVerticies*3];
     int *Indicies = new int[numTriangles*3];
     for(unsigned int i=0;i<numVerticies;i++)
     {
        vert[i*3] = btScalar(collShape->VertexData[i].Position.x);
        vert[i*3+1] = btScalar(collShape->VertexData[i].Position.y);
        vert[i*3+2] = btScalar(collShape->VertexData[i].Position.z);
     }
     for(unsigned int i =0;i<numTriangles;i++)
     {
        Indicies[i*3] = collShape->Indices[i*3];
        Indicies[i*3+1] = collShape->Indices[i*3+1];
        Indicies[i*3+2] = collShape->Indices[i*3+2];
    }
    btGImpactMeshShape* gs = new btGImpactMeshShape(
    new btTriangleIndexVertexArray(numTriangles,
		&Indicies[0],
		3*sizeof(int),
		numVerticies,(btScalar*) &vert[0],sizeof(btScalar)*3)
    );

    gs->setLocalScaling(btVector3(1,1,1));
    gs->setMargin(.07);
    gs->updateBound();
    CollisionShape = btCreateCompoundFromGimpactShape(gs,0.07);
}
void Physics3D::ConstructRigidBody(btScalar mass, glm::vec3 Intertia)
{
    btVector3 inter(Intertia.x,Intertia.y,Intertia.z);
    glm::quat rot = glm::quat(glm::radians(Rotation));
    btTransform trans(btQuaternion(-rot.z,rot.y,-rot.x,rot.w),btVector3(Position.x,Position.y,Position.z));
    dmst = new btDefaultMotionState(trans);
    if(!isStatic)
    {
        CollisionShape->calculateLocalInertia(mass,inter);
    }
    btRigidBody::btRigidBodyConstructionInfo CI(mass, dmst, CollisionShape, inter);
    CI.m_friction = 0.5;
    CI.m_restitution = .6;
    CI.m_rollingFriction = .5;
    RigidBody = new btRigidBody(CI);
    RigidBody->setUserPointer(this);
    RigidBody->setCollisionFlags(RigidBody->getCollisionFlags() |
    btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
}
void Physics3D::ConstructRigidBody(btRigidBody::btRigidBodyConstructionInfo CI)
{
    RigidBody = new btRigidBody(CI);
}
void Physics3D::AddToWorld(PhysicsWorld *addto)
{
    if(RigidBody!=NULL)
    {
        world = addto;
        world->World->addRigidBody(RigidBody);
        return;
    }
    std::cout<<"ERROR: Rigid body not constructed for "<<Name<<std::endl;
}

void Physics3D::removeFromWorld()
{
    world->World->removeRigidBody(RigidBody);
}
bool Physics3D::isCollidedWith(Physics3D *Other)
{
  /* GLOBAL::ObjectCollisionPointer check;
    check.FirstPointer = this;
    check.SecondPointer = Other;
    return GLOBAL::checkCollision(&check);*/
}



void Physics3D::setLinearVelocity(glm::vec3 vel)
{
    if(RigidBody!=NULL)
        RigidBody->setLinearVelocity(btVector3(vel.x,vel.y,vel.z));
}
void Physics3D::setAngularVelocity(glm::vec3 vel)
{
   // if(RigidBody!=NULL)
        RigidBody->setAngularVelocity(btVector3(vel.x,vel.y,vel.z));
}
glm::vec3 Physics3D::getAngularVelocity() const
{
    btVector3 angVel = RigidBody->getAngularVelocity();
    return glm::vec3(angVel.getX(), angVel.getY(), angVel.getZ());
}
glm::vec3 Physics3D::getLinearVelocity() const
{
    btVector3 linVel = RigidBody->getLinearVelocity();
    return glm::vec3(linVel.getX(), linVel.getY(), linVel.getZ());

}


