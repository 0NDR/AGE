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
glm::vec3 *Object3D::getPosition()
{
    return &Position;
}
glm::vec3 *Object3D::getRotation()
{
    return &Rotation;
}
void Object3D::setScale(glm::vec3 pos)
{
    Scale = pos;
}
glm::vec3 *Object3D::getScale()
{
    return &Scale;
}
glm::mat4 Object3D::getAbsoluteMatrix()
{
    glm::mat4 scaltrans, postrans, rottrans, parenttrans;
    glm::quat AbsoluteRotation = glm::quat(glm::radians(Rotation));
    glm::vec3 AbsolutePosition = Position;
    glm::vec3 AbsoluteScale = Scale;
    if(Parent->isType(Object3D::TypeID())&&getParent()!=this)
    {
        Object3D* gParent = (Object3D*)Parent;
        parenttrans = gParent->getAbsoluteMatrix();
    }

    scaltrans =  glm::scale(scaltrans,AbsoluteScale);
    postrans = glm::translate(postrans,AbsolutePosition);
    rottrans = glm::mat4_cast(AbsoluteRotation);

    return parenttrans*(postrans*rottrans*(scaltrans));
}


