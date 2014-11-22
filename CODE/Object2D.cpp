#include "Object2D.h"
#include <typeinfo>

void Object2D::setSize(glm::vec4 newSize){Size = newSize;}
void Object2D::setPosition(glm::vec4 newPos){Position = newPos;}
void Object2D::setRotation(float newRotation){Rotation = newRotation;}

glm::vec4 *Object2D::getSize(){return &Size;}
glm::vec4 *Object2D::getPosition(){return &Position;}
float Object2D::getRotation(){return Rotation;}

glm::vec2 Object2D::getPositionRelative()
{
    return glm::vec2(Position.x,Position.z);
}
glm::vec2 Object2D::getSizeRelative()
{
    return glm::vec2(Size.x,Size.z);
}

glm::mat4 Object2D::getAbsoluteMatrix()
{
    glm::mat4 scaltrans, postrans, rottrans, parenttrans;
    glm::vec2 AbsolutePosition = getPositionRelative();
    glm::vec2 AbsoluteScale = getSizeRelative();
    float AbsoluteRotation = getRotation();
    if(Parent->isType(Object2D::TypeID())&&getParent()!=this)
    {
        Object2D* UIParent = (Object2D*)Parent;
        parenttrans = UIParent->getAbsoluteMatrix();
    }
    if(AbsoluteRotation!=0)
    {
        rottrans = glm::rotate(rottrans,AbsoluteRotation,glm::vec3(0,0,1));
    }
    scaltrans =  glm::scale(scaltrans,glm::vec3(AbsoluteScale+(glm::vec2(Size.y,Size.w)/renderWindow->getSize()),0));
    postrans = glm::translate(postrans,glm::vec3(AbsolutePosition+(glm::vec2(Position.y,Position.w)/renderWindow->getSize()),0));
    return parenttrans*(postrans*(scaltrans)*rottrans);
}
