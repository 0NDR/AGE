#include "GameShader.h"
glm::vec3 GameShader::getViewPosition(){return -glm::vec3(ViewMatrix[3])*glm::mat3(ViewMatrix);}
void GameShader::setViewMatrix(glm::mat4 view)
{
    Activate();
    glUniformMatrix4fv(glGetUniformLocation(ShaderProgram,"view"), 1, GL_FALSE, glm::value_ptr(view));
    ViewMatrix = view;
}

void GameShader::setProjectionMatrix(glm::mat4 proj)
{
    Activate();
    glUniformMatrix4fv(glGetUniformLocation(ShaderProgram,"proj"), 1, GL_FALSE, glm::value_ptr(proj));
    ProjectionMatrix = proj;
}

void GameShader::setProjectionMatrix(float FOV, float AspectRatio, float NearClip, float FarClip)
{
    setProjectionMatrix(glm::perspective(FOV,AspectRatio,NearClip,FarClip));
}

void GameShader::setViewMatrix(glm::vec3 pos, glm::vec3 rotation)
{
    glm::mat4 rottrans,trans;
        rottrans = glm::rotate(rottrans,rotation.x,glm::vec3(1,0,0));
        rottrans = glm::rotate(rottrans,rotation.y,glm::vec3(0,1,0));
        rottrans = glm::rotate(rottrans,rotation.z,glm::vec3(0,0,1));
    trans = glm::translate(trans, pos);
    setViewMatrix(rottrans*trans);
}
