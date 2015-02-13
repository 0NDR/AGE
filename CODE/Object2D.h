#ifndef OBJ2D_H_INCLUDED
#define OBJ2D_H_INCLUDED
#include "Renderable.h"
class Object2D: public Renderable ///Base class for all 2D rendering
{
    protected:

        float Rotation;
    public:
        Object2D(){addNewType(); Rotation=0;}
        Object2D(Object* parent): Renderable(parent){addNewType(); Rotation=0;}
        Object2D(std::string name): Renderable(name){addNewType(); Rotation=0;}
        Object2D(Object* parent, std::string name): Renderable(parent,name){addNewType(); Rotation=0;}

        glm::vec4 Position; ///<The Object2D's position, in the form <Relative X, Absolute X, Relative Y, Absolute Y>
        glm::vec4 Size;     ///<The Object2D's size in the form <Relative size X, Absolute size X, Relative size Y, Absolute size Y>

        void setSize(glm::vec4 newSize); ///<Sets the object's size
        void setPosition(glm::vec4 newPos); ///<Sets the object's position
        void setRotation(float newRotation); ///<Sets the object's rotation, in degrees

        glm::vec4 *getSize();
        glm::vec4 *getPosition();
        float getRotation(); ///<Gets the rotation, in degrees

        glm::vec2 getSizeRelative();///<Returns the object's relative size
        glm::vec2 getPositionRelative(); ///<Returns the object's relative position

        virtual glm::mat4 getAbsoluteMatrix();  ///<Returns the 4x4 matrix containing all transformations in world space
        virtual void Render(){}
        virtual void Update(){}
        virtual int push(lua_State *l)
        {
            luabridge::push(l,this);
            return 1;
        }
        static std::string TypeID() {return "Object2D";}///<Returns Object2D's class name
        virtual std::string type() {return "Object2D";}///<Returns the object's type name
        static void RegisterLua(lua_State *l)
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
