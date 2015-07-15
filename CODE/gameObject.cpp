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
            renderTexture->setUniformLocation(0,"Texture_Diffuse0");
            renderTexture->Activate();
            renderTexture->bindTexture();
            glUniform1i(glGetUniformLocation(ShaderProgram, "Texture_Diffuse0" ),0);
        }
        glUniformMatrix4fv(glGetUniformLocation(ShaderProgram, "model" ),1,GL_FALSE,glm::value_ptr(getAbsoluteMatrix()));
        glUniform2f(glGetUniformLocation(ShaderProgram, "TextureScaling"),TextureScale.x,TextureScale.y);
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
void gameObject::setTextureScale(glm::vec2 scale)
{
    TextureScale = scale;
}
glm::vec2 *gameObject::getTextureScale()
{
    return &TextureScale;
}
void gameObject::setColor(glm::vec4 pos)
{
    Color = pos;
}
glm::vec4 *gameObject::getColor()
{
    return &Color;
}



