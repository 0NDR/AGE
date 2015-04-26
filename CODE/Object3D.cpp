#include "Object3D.h"
#include <typeinfo>

void Object3D::setRotation(glm::vec3 rot)
{
    Rotation = rot;
}
void Object3D::setPosition(glm::vec3 pos)
{
    Position = pos;
}
void Object3D::setSize(glm::vec3 pos)
{
    Size = pos;
}
void Object3D::setRotation(float x, float y, float z)
{
    Rotation = glm::vec3(x,y,z);
}
void Object3D::setPosition(float x, float y, float z)
{
    Position = glm::vec3(x,y,z);
}
void Object3D::setSize(float x, float y, float z)
{
    Size = glm::vec3(x,y,z);
}
glm::vec3 *Object3D::getPosition()
{
    return &Position;
}
glm::vec3 *Object3D::getRotation()
{
    return &Rotation;
}

glm::vec3 *Object3D::getSize()
{
    return &Size;
}
glm::mat4 Object3D::getAbsoluteMatrix()
{
    glm::mat4 scaltrans, postrans, rottrans, parenttrans;
    glm::quat AbsoluteRotation = glm::quat(glm::radians(Rotation));
    glm::vec3 AbsolutePosition = Position;
    glm::vec3 AbsoluteSize = Size;
    if(Parent->isType(Object3D::TypeID())&&getParent()!=this)
    {
        Object3D* gParent = (Object3D*)Parent;
        parenttrans = gParent->getAbsoluteMatrix();
    }

    scaltrans =  glm::scale(scaltrans,AbsoluteSize);
    postrans = glm::translate(postrans,AbsolutePosition);
    rottrans = glm::mat4_cast(AbsoluteRotation);

    return parenttrans*(postrans*rottrans*(scaltrans));
}


