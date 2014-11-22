#ifndef OBJ2D_H_INCLUDED
#define OBJ2D_H_INCLUDED
#include "Renderable.h"
class Object2D: public Renderable
{
    protected:

        float Rotation;
    public:
        Object2D(){addNewType(); Rotation=0;}
        Object2D(Object* parent): Renderable(parent){addNewType(); Rotation=0;}
        Object2D(std::string name): Renderable(name){addNewType(); Rotation=0;}
        Object2D(Object* parent, std::string name): Renderable(parent,name){addNewType(); Rotation=0;}

        void setSize(glm::vec4 newSize);
        void setPosition(glm::vec4 newPos);
        void setRotation(float newRotation);

        glm::vec4 *getSize();
        glm::vec4 *getPosition();
        float getRotation();

        glm::vec4 Position;
        glm::vec4 Size;
        glm::vec2 getSizeRelative();
        glm::vec2 getPositionRelative();

        virtual glm::mat4 getAbsoluteMatrix();
        virtual void Render(){}
        virtual void Update(){}

        virtual int push(lua_State *l)
        {
            luabridge::push(l,this);
            return 1;
        }
        static std::string TypeID() {return "Object2D";}
        virtual std::string type() {return "Object2D";}
        static void RegisterLua(lua_State* l)
        {
            if(!GLOBAL::isRegistered(Renderable::TypeID(),l))
            {
                Renderable::RegisterLua(l);
            }
            GLOBAL::addRegister(TypeID(),l);
            luabridge::getGlobalNamespace(l).deriveClass<Object2D,Renderable>(TypeID().c_str())
                                                .addConstructor<void (*)(std::string)>()
                                                .addProperty("Position",(glm::vec4* (Object2D::*)()const)&Object2D::getPosition,&Object2D::setPosition)
                                                .addProperty("Rotation",(float (Object2D::*)()const)&Object2D::getRotation,&Object2D::setRotation)
                                                .addProperty("Size",(glm::vec4* (Object2D::*)()const)&Object2D::getSize,&Object2D::setSize)
                                            .endClass();
        }
};


#endif // OBJ2D_H_INCLUDED
