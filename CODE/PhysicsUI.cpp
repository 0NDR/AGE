#include "PhysicsUI.h"
#include <typeinfo>

void PhysicsUI::Update()
{
    btTransform trans;
    RigidBody->getMotionState()->getWorldTransform(trans);
    btVector3 newPos = trans.getOrigin();
    btQuaternion transQuat = trans.getRotation();
    btVector3 rot2 = RadiansToDegrees(transQuat.getAxis()*transQuat.getAngle());

    if(newPos.getZ())
    {
        std::cout<<"WARNING: "<<std::endl;
        std::cout<<"Object "<<getName()<<" Is out of 2D!"<<std::endl;
        std::cout<<"------------------------------------"<<std::endl;
    }
    setRotation(-rot2.getZ());
    Position = glm::vec4(newPos.getX(),0,-newPos.getY(),0);
}

float PhysicsUI::getInvMass(){return RigidBody->getInvMass();}
void PhysicsUI::setMass(float m,glm::vec3 inertia){RigidBody->setMassProps(m,btVector3(inertia.x,inertia.y,inertia.z));}
float PhysicsUI::getFriction(){return RigidBody->getFriction();}
void PhysicsUI::setFriction(float m){RigidBody->setFriction(m);}
float PhysicsUI::getRollingFriction(){return RigidBody->getRollingFriction();}
void PhysicsUI::setRollingFriction(float m){RigidBody->setRollingFriction(m);}
float PhysicsUI::getRestitution(){return RigidBody->getRestitution();}
void PhysicsUI::setRestitution(float m){RigidBody->setRestitution(m);}

void PhysicsUI::setPosition(glm::vec4 newPos)
{
    glm::vec4 transAmount = newPos-*getPosition();
    if(RigidBody)
        RigidBody->translate(btVector3(transAmount.x,transAmount.z,0));
    ((UI*)this)->setPosition(newPos);
}
void PhysicsUI::SetCollisionShape(btCollisionShape* col)
{
    CollisionShape = col;
}
void PhysicsUI::SetCollisionShape(Mesh* collShape, bool replaceDuplicates)
{
    btTriangleMesh *triShape = new btTriangleMesh();
    if(collShape->Indices.size()%3==0)
    {
        for(int i =0;i<collShape->Indices.size()/3;i++)
        {
            // For whatever your source of triangles is
            //   give the three points of each triangle:
            glm::vec3 pos = collShape->VertexData[collShape->Indices[i]].Position;
            btVector3 v0(pos.x,pos.y,pos.z);
            pos = collShape->VertexData[collShape->Indices[i+1]].Position;
            btVector3 v1(pos.x,pos.y,pos.z);
            pos = collShape->VertexData[collShape->Indices[i+1]].Position;
            btVector3 v2(pos.x,pos.y,pos.z);
            // Then add the triangle to the mesh:
            triShape->addTriangle(v0,v1,v2);
        }
    }
    else
    {
        std::cout<<"ERROR: Indices not divisible by 3"<<std::endl;
        return;
    }
    CollisionShape = new btConvexTriangleMeshShape(triShape);

}

void PhysicsUI::ConstructRigidBody(btScalar mass, glm::vec3 Intertia)
{
    btVector3 inter(Intertia.x,Intertia.y,Intertia.z);
    btQuaternion rotation;
    rotation.setEuler(0,0,Rotation);
    btTransform trans(rotation,btVector3(Position.x,-Position.z,0));
    dmst = new btDefaultMotionState(trans);
    CollisionShape->calculateLocalInertia(mass,inter);
    btRigidBody::btRigidBodyConstructionInfo CI(mass, dmst, CollisionShape, inter);
    CI.m_friction = 0.9;
    CI.m_restitution = .6;
    CI.m_rollingFriction = .70;
    RigidBody = new btRigidBody(CI);
    RigidBody->setLinearFactor(btVector3(1,1,0));
    RigidBody->setAngularFactor(btVector3(0,0,1));
    RigidBody->setUserPointer(this);
    RigidBody->setCollisionFlags(RigidBody->getCollisionFlags() |
    btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
}
void PhysicsUI::ConstructRigidBody(btRigidBody::btRigidBodyConstructionInfo CI)
{
    RigidBody = new btRigidBody(CI);
}
void PhysicsUI::AddToWorld(btDiscreteDynamicsWorld *addto)
{
    if(RigidBody!=NULL)
    {
        world = addto;
        world->addRigidBody(RigidBody);
        return;
    }
    std::cout<<"ERROR: Rigid body not constructed for "<<Name<<std::endl;
}

void PhysicsUI::removeFromWorld()
{
    world->removeRigidBody(RigidBody);
}

bool PhysicsUI::isCollidedWith(PhysicsUI *Other)
{
    GLOBAL::ObjectCollisionPointer check;
    check.FirstPointer = this;
    check.SecondPointer = Other;
    return GLOBAL::checkCollision(&check);
}
