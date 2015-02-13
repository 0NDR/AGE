#ifndef GSHADER_H_INCLUDED
#define GSHADER_H_INCLUDED
#include "Shader.h"

class GameShader: public Shader ///Expanded shader class for extra control
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

    void setProjectionMatrix(glm::mat4 proj);                                               ///<Set the projection matrix
    void setViewMatrix(glm::mat4 view);                                                     ///<Set the view matrix
    void setProjectionMatrix(float FOV, float AspectRatio, float NearClip, float FarClip);  ///<Set the projection matrix based on inputs
    void setViewMatrix(glm::vec3 pos, glm::vec3 rotation);                                  ///<Set view matrix based on inputs

    glm::mat4 getProjectionMatrix(){return ProjectionMatrix;}                               ///<Get the projection matrix
    glm::mat4 getViewMatrix(){return ViewMatrix;}                                           ///<Get the view matrix
    glm::vec3 getViewPosition(){return viewPos;}                                            ///<Get the position component of the view matrix
    glm::vec3 getViewRotation(){return viewRot;}                                            ///<Get the rotation component of the view matrix
    float getFOV(){return FOV;}                                                             ///<Get the Field of View
    float getAspectRatio(){return AspectRation;}                                            ///<Get the projection aspect ratio
    float getNearClip(){return Near;}                                                       ///<Get the near clipping plane
    float getFarClip(){return Far;}                                                         ///<Get the far clipping plane
};


#endif // GSHADER_H_INCLUDED
