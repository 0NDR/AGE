#include "Billboard.h"
#include "GameShader.h"
void Billboard::Render()
{
        if(renderShader == NULL)
        {
            std::cout<<type()<<" "<<Name<<" is missing a shader"<<std::endl;
            return;
        }
        renderShader->Activate();
        GLuint ShaderProgram = renderShader->ShaderProgram;
        glUniformMatrix4fv(glGetUniformLocation(ShaderProgram, "model" ),1,GL_FALSE,glm::value_ptr(getAbsoluteMatrix()));
        GLint unioverColor = glGetUniformLocation(ShaderProgram,"ObjectColor");
        glUniform4f(unioverColor,Color.x,Color.y,Color.z,Color.w);
        renderMesh->drawToShader(renderShader);
}

glm::mat4 Billboard::getAbsoluteMatrix()
{
    glm::mat4 scaltrans, postrans, rottrans, parenttrans;
    glm::mat4 invViewMatrix, invProjMatrix;
    glm::vec3 viewpos;
    if(renderShader->type() == GameShader::TypeID())
    {
        invViewMatrix = (((GameShader*)renderShader)->getViewMatrix());
        invProjMatrix = ((GameShader*)renderShader)->getProjectionMatrix();
        viewpos = ((GameShader*)renderShader)->getViewPosition();
    }
    else
    {
        std::cout<<"Billboard needs game shader"<<std::endl;
        invViewMatrix = glm::mat4(1.0f);
        invProjMatrix = glm::mat4(1.0f);
        viewpos = glm::vec3(1.0f);
    }
    glm::vec3 AbsolutePosition = Position;
    glm::vec2 AbsoluteScale = getSizeRelative();
    float AbsoluteRotation = getRotation();

    if(Parent->isType(UI::TypeID())&&getParent()!=this)
    {
        UI* UIParent = (UI*)Parent;
        parenttrans = UIParent->getAbsoluteMatrix();
    }
    if(AbsoluteRotation!=0)
    {
        rottrans = glm::rotate(rottrans,AbsoluteRotation,glm::vec3(0,0,1));
    }
    float distanceFromCamera = glm::distance(AbsolutePosition,viewpos);
    scaltrans =  glm::scale(scaltrans,glm::vec3(AbsoluteScale+(glm::vec2(Size.y,Size.w)/renderWindow->getSize()),0)*distanceFromCamera);
    postrans = glm::translate(postrans,AbsolutePosition);
    glm::mat4 finalMat= parenttrans*(postrans*(scaltrans)*rottrans);
    return glm::transpose(glm::mat4(invViewMatrix[0][0],invViewMatrix[0][1],invViewMatrix[0][2],AbsolutePosition.x,
                                    invViewMatrix[1][0],invViewMatrix[1][1],invViewMatrix[1][2],AbsolutePosition.y,
                                    invViewMatrix[2][0],invViewMatrix[2][1],invViewMatrix[2][2],AbsolutePosition.z,
                                    0                  ,0                  ,0                  ,1))*rottrans*scaltrans;


                       }
/*
(a[0][0] b[0][1] c[0][2] d[0][3])
(e[1][0] f[1][1] g[1][2] h[1][3])
(i[2][0] j[2][1] k[2][2] l[2][3])
(m[3][0] n[3][1] o[3][2] p[3][3])

*/
