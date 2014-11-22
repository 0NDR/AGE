#ifndef OBJ3D_H_INCLUDED
#define OBJ3D_H_INCLUDED
#include "Renderable.h"
class Object3D: public Renderable
{
    protected:
    public:
        Object3D(){addNewType();}
        Object3D(Object* parent): Renderable(parent){addNewType();}
        Object3D(std::string name): Renderable(name){addNewType();}
        Object3D(Object* parent, std::string name): Renderable(parent,name){addNewType();}

        glm::mat4 getAbsoluteMatrix();

        void setRotation(glm::vec3 rot);
        void setPosition(glm::vec3 pos);
        void setScale(glm::vec3 pos);
        glm::vec3 *getPosition();
        glm::vec3 *getRotation();
        glm::vec3 *getScale();

        glm::vec3 Position;
        glm::vec3 Rotation;
        glm::vec3 Scale;
        virtual void Render(){}
        virtual void Update(){}

        virtual int push(lua_State *l)
        {
            luabridge::push(l,this);
            return 1;
        }
        static std::string TypeID() {return "Object3D";}
        virtual std::string type() {return "Object3D";}
        static void RegisterLua(lua_State* l)
        {
            if(!GLOBAL::isRegistered(Renderable::TypeID(),l))
            {
                Renderable::RegisterLua(l);
            }
            GLOBAL::addRegister(TypeID(),l);
            luabridge::getGlobalNamespace(l).deriveClass<Object3D,Renderable>(TypeID().c_str())
                                                .addConstructor<void (*)(std::string)>()
                                                .addProperty("Position",(glm::vec3* (Object3D::*)()const)&Object3D::getPosition,&Object3D::setPosition)
                                                .addProperty("Rotation",(glm::vec3* (Object3D::*)()const)&Object3D::getRotation,&Object3D::setRotation)
                                                .addProperty("Scale",(glm::vec3* (Object3D::*)()const)&Object3D::getScale,&Object3D::setScale)
                                            .endClass();
        }


};


#endif // OBJ3D_H_INCLUDED
