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
    glm::mat4 ObjectScaling, ObjectTranslation, ObjectRotation, parenttrans;
    glm::vec2 AbsolutePosition = getPositionRelative()*renderWindow->getSize()+glm::vec2(Position.y,Position.w);
    glm::vec2 AbsoluteScale = getSizeRelative()       +glm::vec2(Size.y,Size.w)/renderWindow->getSize();
    float AbsoluteRotation = getRotation();
    if(AbsoluteRotation!=0)
    {
        ObjectRotation = glm::rotate(glm::mat4(1.0f),AbsoluteRotation,glm::vec3(0,0,1));
    }
    ObjectScaling =  glm::scale(glm::mat4(1.0f),glm::vec3(AbsoluteScale,1));
    ObjectTranslation = glm::translate(glm::mat4(1.0f),glm::vec3(AbsolutePosition,-11));
    glm::mat4 windowScaling = glm::scale(glm::mat4(1.0f),glm::vec3(renderWindow->getSize(),1.f));
    if(Parent->isType(Object2D::TypeID())&&getParent()!=this)
    {
        Object2D* UIParent = (Object2D*)Parent;
        parenttrans = UIParent->getAbsoluteMatrix()*glm::inverse(windowScaling);
    }
    glm::mat4 parentRotScale = glm::mat4(glm::mat3(parenttrans));
    return (parenttrans*ObjectTranslation*(glm::inverse(parentRotScale)*ObjectRotation*parentRotScale*ObjectScaling)*windowScaling);
}
