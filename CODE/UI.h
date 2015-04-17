#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED
#include "Object2D.h"
#include "Shader.h"
#include "Model.h"
class UI: public Object2D  ///Default class for handling 2D rendering, UIs lie along the XZ-plane.
{
    protected:

        Model* renderMesh;
    public:
        UI(){addNewType();}
        UI(Object* parent): Object2D(parent){addNewType();}
        UI(std::string name): Object2D(name){addNewType();}
        UI(Object* parent, std::string name): Object2D(parent,name){addNewType();}


        void setColor(glm::vec4 color);///< Set the UI's color

        glm::vec4* getColor(); ///<Returns a pointer to the color variable
        glm::vec4 Color;       ///<Color data


        void setMesh(Model* newMesh); ///< Sets the mesh for use when rendering
        void Render(Shader* shad)
        {
            setShader(shad);
            this->Render();
        }
        virtual void Render();
        virtual void Update();

        virtual int push(lua_State *l)
        {
            luabridge::push(l,this);
            return 1;
        }
        static std::string TypeID() {return "UI";}///<Return UI's class name
        virtual std::string type() {return "UI";}
        static void RegisterLua(lua_State *l)
        {



            if(!GLOBAL::isRegistered(Object2D::TypeID(),l))
            {
                Object2D::RegisterLua(l);
            }
            GLOBAL::addRegister(TypeID(),l);
            luabridge::getGlobalNamespace(l).deriveClass<UI,Object2D>(TypeID().c_str())
                                                .addConstructor<void (*)(std::string)>()
                                                .addFunction("setMesh",&UI::setMesh)
                                                .addProperty("Color",(glm::vec4* (UI::*)()const)&UI::getColor,&UI::setColor)
                                            .endClass();
    } ///<Adds the class definition to a given lua state
};


#endif // GAMEOBJECT_H_INCLUDED
