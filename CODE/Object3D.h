#ifndef OBJ3D_H_INCLUDED
#define OBJ3D_H_INCLUDED
#include "Renderable.h"
class Object3D: public Renderable///Base class for 3D rendering
{
    protected:
    public:
        Object3D(){addNewType();}
        Object3D(Object* parent): Renderable(parent){addNewType();}
        Object3D(std::string name): Renderable(name){addNewType();}
        Object3D(Object* parent, std::string name): Renderable(parent,name){addNewType();}

        glm::mat4 getAbsoluteMatrix();///<Return the absolute transformation matrix

        void setRotation(glm::vec3 rot); ///<Sets the rotation portion of the transformation matrix
        void setPosition(glm::vec3 pos); ///<Sets the position portion of the transformation matrix
        void setSize(glm::vec3 pos);    ///<Sets the scaling portion of the transformation matrix
        void setRotation(float x, float y, float z); ///<Sets the rotation portion of the transformation matrix
        void setPosition(float x, float y, float z); ///<Sets the position portion of the transformation matrix
        void setSize(float x, float y, float z);    ///<Sets the scaling portion of the transformation matrix
        glm::vec3 *getPosition();
        glm::vec3 *getRotation();
        glm::vec3 *getSize();

        glm::vec3 Position; ///<The set position
        glm::vec3 Rotation; ///<The set rotation
        glm::vec3 Size;    ///<The set scale
        virtual void Render(){}
        virtual void Update(){}

        virtual int push(lua_State *l)
        {
            luabridge::push(l,this);
            return 1;
        }
        static std::string TypeID() {return "Object3D";}
        virtual std::string type() {return "Object3D";}
        static void RegisterLua(lua_State *l)
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
                                                .addProperty("Size",(glm::vec3* (Object3D::*)()const)&Object3D::getSize,&Object3D::setSize)
                                            .endClass();
    } ///<Adds the class definition to a given lua state


};


#endif // OBJ3D_H_INCLUDED
