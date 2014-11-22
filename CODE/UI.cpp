#include "UI.h"
#include <typeinfo>

void UI::setColor(glm::vec4 pos){Color = pos;}

glm::vec4 *UI::getColor(){return &Color;}

void UI::setMesh(Mesh* newMesh)
{
    renderMesh = newMesh;
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
        glDisable(GL_DEPTH_TEST);
        GLuint ShaderProgram = renderShader->ShaderProgram;
        GLint pA = glGetAttribLocation(ShaderProgram, "position");
        GLint teA = glGetAttribLocation(ShaderProgram, "texcoord");
        glUniformMatrix4fv(glGetUniformLocation(ShaderProgram, "model" ),1,GL_FALSE,glm::value_ptr(getAbsoluteMatrix()));

        GLint unioverColor = glGetUniformLocation(ShaderProgram,"ObjectColor");
        glUniform4f(unioverColor,Color.x,Color.y,Color.z,Color.w);

        glEnableVertexAttribArray(pA);
        glEnableVertexAttribArray(teA);
        for (unsigned int i = 0 ; i < renderMesh->MeshDataEntries.size() ; i++) {
            glBindBuffer(GL_ARRAY_BUFFER, renderMesh->MeshDataEntries[i].VertexBuffer);
            glVertexAttribPointer(pA, 3, GL_FLOAT, GL_FALSE,  sizeof(GLOBAL::Vertex), 0);
            glVertexAttribPointer(teA, 2, GL_FLOAT, GL_FALSE,  sizeof(GLOBAL::Vertex), (const GLvoid*)24);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderMesh->MeshDataEntries[i].IndexBuffer);
            glDrawElements(GL_TRIANGLES, renderMesh->MeshDataEntries[i].IndiciesBufferLength, GL_UNSIGNED_INT, 0);
        }
    glDisableVertexAttribArray(pA);
    glDisableVertexAttribArray(teA);
    glEnable(GL_DEPTH_TEST);
}
