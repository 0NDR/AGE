#include "gameObject.h"
#include <typeinfo>
void gameObject::Render()
{
        if(renderShader == NULL)
        {
            std::cout<<"GameObject "<<Name<<" is missing a shader"<<std::endl;
            return;
        }
        renderShader->Activate();
        GLuint ShaderProgram = renderShader->ShaderProgram;
        GLint unioverColor = glGetUniformLocation(ShaderProgram,"ObjectColor");
        glUniform4f(unioverColor,Color.x,Color.y,Color.z,Color.w);
        if(renderTexture!=NULL)
        {
            renderTexture->Activate();
            renderTexture->bindTexture();
            glUniform1i(glGetUniformLocation(ShaderProgram, "ObjectTexture" ),0);
        }
        glUniformMatrix4fv(glGetUniformLocation(ShaderProgram, "model" ),1,GL_FALSE,glm::value_ptr(getAbsoluteMatrix()));

        glUniform3f(glGetUniformLocation(ShaderProgram, "scale"),Scale.x,Scale.y,Scale.z);
        renderMesh->drawToShader(renderShader);
}
void gameObject::Update()
{

}

void gameObject::setMesh(Model* newMesh)
{
    renderMesh = newMesh;
}
void gameObject::setTexture(glTexture *texture)
{
    renderTexture = texture;
}

void gameObject::setColor(glm::vec4 pos)
{
    Color = pos;
}
glm::vec4 *gameObject::getColor()
{
    return &Color;
}



