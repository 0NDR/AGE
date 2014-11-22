#ifndef RENDERABLE_H_INCLUDED
#define RENDERABLE_H_INCLUDED
#include "Object.h"
#include "VideoController.h"
#include "Shader.h"
class Renderable: public Object
{
    protected:
        Window* renderWindow;
        Shader* renderShader;
    public:
    Renderable(){addNewType(); }
    Renderable(Object* parent): Object(parent){addNewType();}
    Renderable(std::string name): Object(name){addNewType();}
    Renderable(Object* parent, std::string name): Object(parent,name){addNewType();}
    void setWindow(Window* newWindow);
    void setShader(Shader* newShader);
    void Render(Shader* shad)
    {
        setShader(shad);
        this->Render();
    }
    virtual void Render()=0;
    virtual void Update()=0;

    static std::string TypeID() {return "Renderable";}
    virtual std::string type() {return "Renderable";}
    static void RegisterLua(lua_State* l)
    {
            if(!GLOBAL::isRegistered(Object::TypeID(),l))
            {
                Object::RegisterLua(l);
            }
            GLOBAL::addRegister(Renderable::TypeID(),l);
            luabridge::getGlobalNamespace(l).deriveClass<Renderable,Object>(TypeID().c_str())
                                                .addFunction("setShader",&Renderable::setShader)
                                            .endClass();
    }
};


#endif // RENDERABLE_H_INCLUDED
