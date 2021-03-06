#ifndef GAMEOBJECT_H_INCLUDED
#define GAMEOBJECT_H_INCLUDED
#include "Object3D.h"
#include "Model.h"
#include "glTexture.h"
#include "Shader.h"
class gameObject: public Object3D   ///Extended class for rendering 3D meshs
{
    protected:

    private:
        glm::mat4 AbsoluteMatrix;
        Model *renderMesh;
        glTexture *renderTexture = NULL;
    public:
        gameObject(){addNewType();TextureScale=glm::vec2(1,1);}
        gameObject(Object* parent): Object3D(parent){addNewType();TextureScale=glm::vec2(1,1);}
        gameObject(std::string name): Object3D(name){addNewType();TextureScale=glm::vec2(1,1);}
        gameObject(Object* parent, std::string name): Object3D(parent,name){addNewType();TextureScale=glm::vec2(1,1);}

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
        static std::string TypeID() {return "gameObject";}
        virtual std::string type() {return "gameObject";}
        static void RegisterLua(lua_State *l)
        {

            if(!GLOBAL::isRegistered(Object3D::TypeID(),l))
            {
                Object3D::RegisterLua(l);
            }
            GLOBAL::addRegister(TypeID(),l);
            luabridge::getGlobalNamespace(l).deriveClass<gameObject,Object3D>(TypeID().c_str())
                                                .addConstructor<void (*)(std::string)>()
                                                .addFunction("setMesh",&gameObject::setMesh)
                                                .addFunction("setTexture", &gameObject::setTexture)
                                                .addProperty("Color",(glm::vec4* (gameObject::*)()const)&gameObject::getColor,&gameObject::setColor)
                                            .endClass();
    } ///<Adds the class definition to a given lua state


};


#endif // GAMEOBJECT_H_INCLUDED
