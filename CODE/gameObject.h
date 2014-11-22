#ifndef GAMEOBJECT_H_INCLUDED
#define GAMEOBJECT_H_INCLUDED
#include "Object3D.h"
#include "Mesh.h"
#include "glTexture.h"
#include "Shader.h"
class gameObject: public Object3D
{
    protected:

    private:
        glm::mat4 AbsoluteMatrix;
        Mesh *renderMesh;
        glTexture *renderTexture = NULL;
    public:
        gameObject(){addNewType();}
        gameObject(Object* parent): Object3D(parent){addNewType();}
        gameObject(std::string name): Object3D(name){addNewType();}
        gameObject(Object* parent, std::string name): Object3D(parent,name){addNewType();}

        virtual void Update();
        virtual void Render();
        virtual void Render(Shader* shad)
        {
            std::cout<<"gameObject render"<<std::endl;
            setShader(shad);
            this->Render();
        }
        void setMesh(Mesh *newMesh);
        void setTexture(glTexture *texture);

        void setColor(glm::vec4 pos);
        glm::vec4 *getColor();
        glm::vec4 Color;


        virtual int push(lua_State *l)
        {
            luabridge::push(l,this);
            return 1;
        }
        static std::string TypeID() {return "gameObject";}
        virtual std::string type() {return "gameObject";}
        static void RegisterLua(lua_State* l)
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
        }


};


#endif // GAMEOBJECT_H_INCLUDED
