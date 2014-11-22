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
        GLint pA = glGetAttribLocation(ShaderProgram, "position");
        GLint nA = glGetAttribLocation(ShaderProgram, "normal");
        GLint teA = glGetAttribLocation(ShaderProgram, "texcoord");
        GLint taA = glGetAttribLocation(ShaderProgram, "tangent");
        GLint btA = glGetAttribLocation(ShaderProgram, "bitangent");
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
        glEnableVertexAttribArray(pA);
        glEnableVertexAttribArray(nA);
        glEnableVertexAttribArray(teA);
        glEnableVertexAttribArray(taA);
        glEnableVertexAttribArray(btA);
        for (unsigned int i = 0 ; i < renderMesh->MeshDataEntries.size() ; i++) {
            glBindBuffer(GL_ARRAY_BUFFER, renderMesh->MeshDataEntries[i].VertexBuffer);
            glVertexAttribPointer(pA, 3, GL_FLOAT, GL_FALSE,  sizeof(GLOBAL::Vertex), 0);
            glVertexAttribPointer(nA, 3, GL_FLOAT, GL_FALSE,  sizeof(GLOBAL::Vertex), (const GLvoid*)12);
            glVertexAttribPointer(teA, 2, GL_FLOAT, GL_FALSE,  sizeof(GLOBAL::Vertex), (const GLvoid*)24);
            glVertexAttribPointer(taA, 3, GL_FLOAT, GL_FALSE,  sizeof(GLOBAL::Vertex), (const GLvoid*)32);
            glVertexAttribPointer(btA, 3, GL_FLOAT, GL_FALSE,  sizeof(GLOBAL::Vertex), (const GLvoid*)44);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderMesh->MeshDataEntries[i].IndexBuffer);
            glDrawElements(GL_TRIANGLES, renderMesh->MeshDataEntries[i].IndiciesBufferLength, GL_UNSIGNED_INT, 0);
        }
    glDisableVertexAttribArray(pA);
    glDisableVertexAttribArray(nA);
    glDisableVertexAttribArray(teA);
    glDisableVertexAttribArray(taA);
    glDisableVertexAttribArray(btA);
}
void gameObject::Update()
{

}

void gameObject::setMesh(Mesh* newMesh)
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



