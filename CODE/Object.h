#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#include "GLOBAL.h"

class Object
{


    public:
        Object();
        Object(Object* parent);
        Object(std::string name);
        Object(Object* parent, std::string name);
        std::vector<std::string> parenttypes;
        std::string getName() const;
        Object* getParent();
        Object* findFirstChild(std::string childName);
        std::vector<Object*> getChildArray();
        void removeChild(Object* newChild);
        void addChild(Object* newChild);
        void setName(std::string newName);
        void setParent(Object* newParent);
        bool isType(std::string other);
        bool luaIsEqual(Object* other);

        int findFirstChildLua(lua_State *k);
        int getParentLua(lua_State *l);
        int getChildArrayLua(lua_State *k);

        static std::string TypeID() {return "Object";}
        virtual std::string type() {return "Object";}

        virtual int push(lua_State *l)
        {
            luabridge::push(l,this);
            return 1;
        }
        static void RegisterLua(lua_State* l)
        {
            GLOBAL::addRegister(Object::TypeID(),l);
            luabridge::getGlobalNamespace(l).beginClass<Object>(Object::TypeID().c_str())
                                            .addConstructor<void (*)(std::string)>()
                                            .addProperty("Name", &Object::getName,&Object::setName)
                                            .addCFunction("getChildren",&Object::getChildArrayLua)
                                            .addProperty("Parent", (Object& (Object::*)() const)&Object::getParent,&Object::setParent)
                                            .addCFunction("getParent",&Object::getParentLua)
                                            .addCFunction("findFirstChild",&Object::findFirstChildLua)
                                            .addFunction("isA",&Object::isType)
                                            .addFunction("__eq",(bool (Object::*) (const Object*) const)&Object::luaIsEqual)
                                            .addFunction("type",&Object::type)
                                        .endClass();

        }
   protected:
        std::vector<Object*> children;
        virtual void addNewType()
        {
            parenttypes.push_back(type());
        };
        Object* Parent = NULL;
        std::string Name;
};


#endif // OBJECT_H_INCLUDED
