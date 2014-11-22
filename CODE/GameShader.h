#ifndef GSHADER_H_INCLUDED
#define GSHADER_H_INCLUDED
#include "Shader.h"

class GameShader: public Shader
{
    private:
        glm::mat4 ProjectionMatrix;
        glm::mat4 ViewMatrix;
        glm::vec3 viewPos;
        glm::vec3 viewRot;
        float FOV;
        float AspectRation;
        float Near;
        float Far;
    public:

    GameShader(){ DefaultConstructor(); addNewType();}
    GameShader(Object* parent): Shader(parent){DefaultConstructor(); addNewType();}
    GameShader(std::string name): Shader(name){DefaultConstructor(); addNewType();}
    GameShader(Object* parent, std::string name): Shader(parent,name){DefaultConstructor(); addNewType();}

    void setProjectionMatrix(glm::mat4 proj);
    void setViewMatrix(glm::mat4 view);
    void setProjectionMatrix(float FOV, float AspectRatio, float NearClip, float FarClip);
    void setViewMatrix(glm::vec3 pos, glm::vec3 rotation);

    glm::mat4 getProjectionMatrix(){return ProjectionMatrix;}
    glm::mat4 getViewMatrix(){return ViewMatrix;}
    glm::vec3 getViewPosition(){return viewPos;}
    glm::vec3 getViewRotation(){return viewRot;}
    float getFOV(){return FOV;}
    float getAspectRatio(){return AspectRation;}
    float getNearClip(){return Near;}
    float getFarClip(){return Far;}
};


#endif // GSHADER_H_INCLUDED
