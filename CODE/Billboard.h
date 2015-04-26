#ifndef BILLBOARD_H
#define BILLBOARD_H
#include "Object2D.h"
class Billboard: public Object2D ///Base class for rendering billboards (3D position, 2D size/rotation)
{
    private:
    public:
        Billboard(){addNewType();lockAxis = glm::vec3(0,0,0);}
        Billboard(Object* parent): Object2D(parent){addNewType();lockAxis = glm::vec3(0,0,0);}
        Billboard(std::string name): Object2D(name){addNewType();lockAxis = glm::vec3(0,0,0);}
        Billboard(Object* parent, std::string name): Object2D(parent,name){addNewType();lockAxis = glm::vec3(0,0,0);}

        glm::vec3 lockAxis = glm::vec3(0,0,0);
        glm::vec3 Position;
        glm::vec3 *getPosition(){return &Position;};
        glm::vec3 *getLockAxis(){return &lockAxis;};
        void setPosition(float x, float y, float z); ///<Sets the position portion of the transformation matrix
        void setPosition(glm::vec3 pos); ///<Sets the position portion of the transformation matrix
        void setLockAxis(glm::vec3 pos); ///<Sets the axis which the billboard turns on.

        virtual glm::mat4 getAbsoluteMatrix();
        virtual void Render(){}
        virtual void Update(){}

        virtual int push(lua_State *l)
        {
            luabridge::push(l,this);
            return 1;
        }
        static std::string TypeID() {return "Billboard";}
        virtual std::string type() {return "Billboard";}
        static void RegisterLua(lua_State *l)
        {



            if(!GLOBAL::isRegistered(Object2D::TypeID(),l))
            {
                Object2D::RegisterLua(l);
            }
            GLOBAL::addRegister(TypeID(),l);
            luabridge::getGlobalNamespace(l).deriveClass<Billboard,Object2D>(TypeID().c_str())
                                                .addConstructor<void (*)(std::string)>()
                                                .addProperty("Position",(glm::vec3* (Billboard::*)()const)&Billboard::getPosition,&Billboard::setPosition)
                                                .addProperty("lockAxis",(glm::vec3* (Billboard::*)()const)&Billboard::getLockAxis,&Billboard::setLockAxis)
                                            .endClass();
    } ///<Adds the class definition to a given lua state
};


#endif // BILLBOARD_H
