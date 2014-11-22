#ifndef UI_H_INCLUDED
#define UI_H_INCLUDED
#include "Object2D.h"
#include "Shader.h"
#include "Mesh.h"
class UI: public Object2D
{
    protected:

        Mesh* renderMesh;
    public:
        UI(){addNewType();}
        UI(Object* parent): Object2D(parent){addNewType();}
        UI(std::string name): Object2D(name){addNewType();}
        UI(Object* parent, std::string name): Object2D(parent,name){addNewType();}


        void setColor(glm::vec4 color);

        glm::vec4* getColor();
        glm::vec4 Color;


        void setMesh(Mesh* newMesh);
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
        static std::string TypeID() {return "UI";}
        virtual std::string type() {return "UI";}
        static void RegisterLua(lua_State* l)
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
        }
};


#endif // GAMEOBJECT_H_INCLUDED
