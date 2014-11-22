#ifndef LIGHT_H_INCLUDED
#define LIGHT_H_INCLUDED
#include "Object3D.h"
#include "Shader.h"
class Light: public Object3D
{
    private:

    public:
        Light(){addNewType();}
        Light(Object* parent): Object3D(parent){addNewType();}
        Light(std::string name): Object3D(name){addNewType();}
        Light(Object* parent, std::string name): Object3D(parent,name){addNewType();}
        virtual void Render();
        virtual void Render(Shader* shad)
        {
            std::cout<<"light render"<<std::endl;
            setShader(shad);
            this->Render();
        }
        glm::vec3 Direction;
        glm::vec4 DiffuseColor;
        glm::vec4 AmbientColor;
        glm::vec4 SpecularColor;
        float ConstantAttenuation;
        float LinearAttenuation;
        float QuadraticAttenuation;
        float SpotCutoff;
        float SpotExponent;

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
        }
};

extern std::vector<Light*> Lights;

#endif // GAMEOBJECT_H_INCLUDED
