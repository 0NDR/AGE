#ifndef LIGHT_H_INCLUDED
#define LIGHT_H_INCLUDED
#include "Object3D.h"
#include "Shader.h"
class Light: public Object3D ///Class to manage OpenGL lights
{
    private:
        void setDefaults()
        {
            Direction = glm::vec3(0,0,0);
            DiffuseColor = glm::vec4(0,0,0,1);
            AmbientColor = glm::vec4(0,0,0,1);
            SpecularColor = glm::vec4(0,0,0,1);
            ConstantAttenuation = 1;
            LinearAttenuation = 0;
            QuadraticAttenuation = 0;
            SpotCutoff = 180;
            SpotExponent = 0;
        }
    public:
        Light(){addNewType();setDefaults();}
        Light(Object* parent): Object3D(parent){addNewType();setDefaults();}
        Light(std::string name): Object3D(name){addNewType();setDefaults();}
        Light(Object* parent, std::string name): Object3D(parent,name){addNewType();setDefaults();}
        virtual void Render();
        virtual void Render(Shader* shad)
        {
            setShader(shad);
            this->Render();
        }
        glm::vec3 Direction;        ///<The direction of the light
        glm::vec4 DiffuseColor;     ///<The diffuse color, based on angle the light hits object
        glm::vec4 AmbientColor;     ///<The ambient color, based only on distance
        glm::vec4 SpecularColor;    ///<The specular color, based on how the light bounces off the object into an observer's eyes
        float ConstantAttenuation;  ///<The Constant attenuation, C in C+L(distance)+Q(distance^2)
        float LinearAttenuation;    ///<The Linear attenuation, L in C+L(distance)+Q(distance^2)
        float QuadraticAttenuation; ///<The Quadratic attenuation, Q in C+L(distance)+Q(distance^2)
        float SpotCutoff;           ///<Spotlight cuttof angle, half of total angle
        float SpotExponent;         ///<How fast light should diffuse as it approaches the cut off

        static std::string typeID() {return "Light";}
        virtual std::string type() {return "Light";}
        static void RegisterLua(lua_State* l, bool InitParentType = false)
        {
            if(!GLOBAL::isRegistered(Object3D::TypeID(),l))
            {
                Object3D::RegisterLua(l);
            }
            GLOBAL::addRegister(Light::TypeID(),l);
            luabridge::getGlobalNamespace(l).deriveClass<Light,Object3D>(TypeID().c_str())
                                                .addConstructor<void (*)(std::string)>()
                                                .addData("Direction",&Light::Direction)
                                                .addData("DiffuseColor", &Light::DiffuseColor)
                                                .addData("AmbientColor", &Light::AmbientColor)
                                                .addData("SpecularColor", &Light::SpecularColor)
                                                .addData("ConstantAttenuation", &Light::ConstantAttenuation)
                                                .addData("QuadraticAttenuation", &Light::QuadraticAttenuation)
                                                .addData("LinearAttenuation",&Light::LinearAttenuation)
                                                .addData("SpotCutoff",&Light::SpotCutoff)
                                                .addData("SpotExponent",&Light::SpotExponent)
                                            .endClass();
    } ///<Adds the class definition to a given lua state
};

extern std::vector<Light*> Lights;

#endif // GAMEOBJECT_H_INCLUDED
