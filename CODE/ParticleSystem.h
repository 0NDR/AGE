#ifndef particlesystem_H_INCLUDED
#define particlesystem_H_INCLUDED
#include "Object3D.h"
#include "Model.h"
#include "glTexture.h"
#include "Shader.h"
#include "Particle.h"
class ParticleSystem: public Object3D   ///Extended class for rendering 3D meshs
{
    protected:

    private:

    public:
        ParticleSystem(){addNewType();TextureScale=glm::vec2(1,1);}
        ParticleSystem(Object* parent): Object3D(parent){addNewType();TextureScale=glm::vec2(1,1);}
        ParticleSystem(std::string name): Object3D(name){addNewType();TextureScale=glm::vec2(1,1);}
        ParticleSystem(Object* parent, std::string name): Object3D(parent,name){addNewType();TextureScale=glm::vec2(1,1);}

        virtual void Update();
        virtual void Render();
        virtual void Render(Shader* shad)
        {
            setShader(shad);
            this->Render();
        }
        void setMesh(Model *newMesh);  ///<Sets the gameObject's mesh
        void setTexture(glTexture *texture); ///<Sets the gameObject's texture

        void setColor(glm::vec4 pos); ///<Sets the gameObject's color
        void setTextureScale(glm::vec2 scale);
        glm::vec4 *getColor();
        glm::vec2 *getTextureScale();
        glm::vec4 Color;              ///<The set color
        glm::vec2 TextureScale;


        virtual int push(lua_State *l)
        {
            luabridge::push(l,this);
            return 1;
        }
        static std::string TypeID() {return "ParticleSystem";}
        virtual std::string type() {return "ParticleSystem";}
        static void RegisterLua(lua_State *l)
        {

            if(!GLOBAL::isRegistered(Object3D::TypeID(),l))
            {
                Object3D::RegisterLua(l);
            }
            GLOBAL::addRegister(TypeID(),l);
            luabridge::getGlobalNamespace(l).deriveClass<ParticleSystem,Object3D>(TypeID().c_str())
                                                .addConstructor<void (*)(std::string)>()
                                                .addFunction("setMesh",&ParticleSystem::setMesh)
                                                .addFunction("setTexture", &ParticleSystem::setTexture)
                                                .addProperty("Color",(glm::vec4* (ParticleSystem::*)()const)&ParticleSystem::getColor,&ParticleSystem::setColor)
                                            .endClass();
    } ///<Adds the class definition to a given lua state


};


#endif // particlesystem_H_INCLUDED
