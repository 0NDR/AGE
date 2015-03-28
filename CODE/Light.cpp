#include "Light.h"
#include <typeinfo>
std::vector<Light*> Lights;
void Light::Render()
{
    GLint ShaderProgram = renderShader->ShaderProgram;
    Lights.push_back(this);
    float DiffuseT[Lights.size()*4];
    float AmbientT[Lights.size()*4];
    float SpecularT[Lights.size()*4];
    float PositionT[Lights.size()*3];
    float DirectionT[Lights.size()*3];
    float CAttenT[Lights.size()];
    float LAttenT[Lights.size()];
    float QAttenT[Lights.size()];
    float SCUTT[Lights.size()];
    float SEXPT[Lights.size()];
    for(int i=0;i<Lights.size();++i){
        DiffuseT[i*4] = Lights[i]->DiffuseColor.x;
        DiffuseT[i*4+1] = Lights[i]->DiffuseColor.y;
        DiffuseT[i*4+2] = Lights[i]->DiffuseColor.z;
        DiffuseT[i*4+3] = Lights[i]->DiffuseColor.w;
        AmbientT[i*4] = Lights[i]->AmbientColor.x;
        AmbientT[i*4+1] = Lights[i]->AmbientColor.y;
        AmbientT[i*4+2] = Lights[i]->AmbientColor.z;
        AmbientT[i*4+3] = Lights[i]->AmbientColor.w;
        SpecularT[i*4] = Lights[i]->SpecularColor.x;
        SpecularT[i*4+1] = Lights[i]->SpecularColor.y;
        SpecularT[i*4+2] = Lights[i]->SpecularColor.z;
        SpecularT[i*4+3] = Lights[i]->SpecularColor.w;

        PositionT[i*3] = Lights[i]->Position.x;
        PositionT[i*3+1] = Lights[i]->Position.y;
        PositionT[i*3+2] = Lights[i]->Position.z;
        DirectionT[i*3] = Lights[i]->Direction.x;
        DirectionT[i*3+1] = Lights[i]->Direction.y;
        DirectionT[i*3+2] = Lights[i]->Direction.z;
        CAttenT[i] = Lights[i]->ConstantAttenuation;
        LAttenT[i] = Lights[i]->LinearAttenuation;
        QAttenT[i] = Lights[i]->QuadraticAttenuation;
        SCUTT[i] = cosf(DegreesToRadians(Lights[i]->SpotCutoff)); ///Convert from degrees into cos of the angle, for faster pixel shader
        SEXPT[i] = Lights[i]->SpotExponent;
    }
    glUniform1i(glGetUniformLocation(ShaderProgram,"lightCount"),Lights.size());
    glUniform4fv(glGetUniformLocation(ShaderProgram,"LightDiffuses"),Lights.size()*4,DiffuseT);
    glUniform4fv(glGetUniformLocation(ShaderProgram,"LightAmbients"),Lights.size()*4,AmbientT);
    glUniform4fv(glGetUniformLocation(ShaderProgram,"LightSpeculars"),Lights.size()*4,SpecularT);
    glUniform3fv(glGetUniformLocation(ShaderProgram,"LightPositions"),Lights.size()*3,PositionT);
    glUniform3fv(glGetUniformLocation(ShaderProgram,"LightDirections"),Lights.size()*3,DirectionT);
    glUniform1fv(glGetUniformLocation(ShaderProgram,"LightConstantAttenuations"),Lights.size(),CAttenT);
    glUniform1fv(glGetUniformLocation(ShaderProgram,"LightLinearAttenuations"),Lights.size(),LAttenT);
    glUniform1fv(glGetUniformLocation(ShaderProgram,"LightQuadraticAttenuations"),Lights.size(),QAttenT);
    glUniform1fv(glGetUniformLocation(ShaderProgram,"LightCutoffs"),Lights.size(),SCUTT);
    glUniform1fv(glGetUniformLocation(ShaderProgram,"LightSpotExponents"),Lights.size(),SEXPT);
}
