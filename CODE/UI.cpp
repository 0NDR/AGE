#include "UI.h"
#include <typeinfo>

void UI::setColor(glm::vec4 pos){Color = pos;}

glm::vec4 *UI::getColor(){return &Color;}

void UI::setMesh(Model* newMesh)
{
    renderMesh = newMesh;
}
void UI::setTexture(glTexture* text)
{
    renderTexture=text;
}
void UI::Update()
{

}
void UI::Render()
{
        if(renderShader == NULL)
        {
            std::cout<<type()<<" "<<Name<<" is missing a shader"<<std::endl;
            return;
        }
        renderShader->Activate();
        if(renderTexture!=NULL)
        {
            renderTexture->setUniformLocation(0,"texture0");
            renderTexture->Activate();
            renderTexture->bindTexture();
        }
        GLuint ShaderProgram = renderShader->ShaderProgram;
        glUniform1i(glGetUniformLocation(ShaderProgram,"zIndex"),zIndex);
        glUniform2f(glGetUniformLocation(ShaderProgram,"textureScale"),textureScale.x,textureScale.y);
        glUniformMatrix4fv(glGetUniformLocation(ShaderProgram, "model" ),1,GL_FALSE,glm::value_ptr(getAbsoluteMatrix()));
        GLint unioverColor = glGetUniformLocation(ShaderProgram,"ObjectColor");
        glUniform4f(unioverColor,Color.x,Color.y,Color.z,Color.w);
        renderMesh->drawToShader(renderShader);
}
