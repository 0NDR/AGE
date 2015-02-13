#ifndef BILLBOARD_H
#define BILLBOARD_H
#include "UI.h"
class Billboard: public UI
{
    public:
        Billboard(){addNewType();}
        Billboard(Object* parent): UI(parent){addNewType();}
        Billboard(std::string name): UI(name){addNewType();}
        Billboard(Object* parent, std::string name): UI(parent,name){addNewType();}

        glm::vec3 Position;
        glm::vec3 *getPosition(){return &Position;};
        void setPosition(glm::vec3 a){Position = a;};

        virtual glm::mat4 getAbsoluteMatrix();
        virtual void Render();

        virtual int push(lua_State *l)
        {
            luabridge::push(l,this);
            return 1;
        }
        static std::string TypeID() {return "Billboard";}
        virtual std::string type() {return "Billboard";}
        static void RegisterLua(lua_State *l)
        {



            if(!GLOBAL::isRegistered(UI::TypeID(),l))
            {
                UI::RegisterLua(l);
            }
            GLOBAL::addRegister(TypeID(),l);
            luabridge::getGlobalNamespace(l).deriveClass<Billboard,UI>(TypeID().c_str())
                                                .addConstructor<void (*)(std::string)>()
                                                .addProperty("Position",(glm::vec3* (Billboard::*)()const)&Billboard::getPosition,&Billboard::setPosition)
                                            .endClass();
    } ///<Adds the class definition to a given lua state
};


#endif // BILLBOARD_H
