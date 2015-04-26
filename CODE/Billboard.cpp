#include "Billboard.h"
#include "GameShader.h"
#include "Object3D.h"
void Billboard::setPosition(float x, float y, float z)
{
    setPosition(glm::vec3(x,y,z));
}
void Billboard::setPosition(glm::vec3 pos)
{
    Position= pos;
}
void Billboard::setLockAxis(glm::vec3 pos)
{
    lockAxis = pos;
}
glm::vec3 NormalizeFix(glm::vec3 a)
{
    if(a==glm::vec3(0,0,0))
        return a;
    return glm::normalize(a);
}
glm::mat4 Billboard::getAbsoluteMatrix()
{
    glm::mat4 scaltrans, postrans, rottrans, parenttrans;
    glm::mat4 invViewMatrix;
    glm::vec3 AbsolutePosition = Position;
    glm::vec2 AbsoluteScale = getSizeRelative()+(Size.y,Size.w)/renderWindow->getSize();
    glm::vec3 viewpos;
    if(renderShader->type() == GameShader::TypeID())
    {
        invViewMatrix = (((GameShader*)renderShader)->getViewMatrix());
        viewpos = ((GameShader*)renderShader)->getViewPosition();
    }
    else
    {
        std::cout<<"Billboard needs game shader"<<std::endl;
    }
    float AbsoluteRotation = getRotation();

    if(Parent->isType(Object3D::TypeID())&&getParent()!=this)
    {
        Object3D* Object3DParent = (Object3D*)Parent;
        parenttrans = Object3DParent->getAbsoluteMatrix();
    }

    glm::vec3 zAxis = NormalizeFix(viewpos-AbsolutePosition);
    glm::vec3 yAxis = lockAxis;
    if(yAxis == glm::vec3(0,0,0))
        yAxis = NormalizeFix(glm::cross(zAxis,glm::vec3(0,1,0)));
    glm::vec3 xAxis = NormalizeFix(glm::cross(yAxis,zAxis));

    glm::mat4 towardsCamera = glm::mat4(glm::mat3(xAxis,yAxis,zAxis));
    if(AbsoluteRotation!=0)
    {
        rottrans = glm::rotate(rottrans,AbsoluteRotation,zAxis);
    }
    scaltrans =  glm::scale(scaltrans,glm::vec3(AbsoluteScale,1));
    postrans = glm::translate(postrans,AbsolutePosition);
    glm::mat4 finalMat= parenttrans*(postrans*(scaltrans)*rottrans);
    return postrans*towardsCamera*rottrans*scaltrans;


}

