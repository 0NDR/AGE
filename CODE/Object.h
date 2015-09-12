#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED
#include "GLOBAL.h"
class Object///Base class to all interactive objects
{
   protected:
        std::vector<Object*> children;
        virtual void addNewType()
        {
            parenttypes.push_back(type());
        };
        Object* Parent = NULL;
        std::string Name;
    public:
        Object();
        Object(Object* parent);
        Object(std::string name);
        Object(Object* parent, std::string name);
        std::vector<std::string> parenttypes;
        std::string getName() const;///< Returns the object's name
        Object* getParent(); ///< Returns a pointer to the parent object
        Object* findFirstChild(std::string childName); ///<Returns the child with the specified name, if there are more than one it returns the first it found
        std::vector<Object*> getChildArray();///<Returns a vector full of pointers to children objects
        void removeChild(Object* newChild); ///< Removes the specified child object
        void addChild(Object* newChild);   ///< Adds the specified child
        void setName(std::string newName); ///< Sets the object's name
        void setParent(Object* newParent); ///< Sets the object's parent
        bool isType(std::string other);    ///< Returns true if the child is or is derived from a class name

        bool luaIsEqual(Object* other);
        int findFirstChildLua(lua_State *k);
        int getParentLua(lua_State *l);
        int getChildArrayLua(lua_State *k);

        static std::string TypeID() {return "Object";} ///<Returns Object's class name
        virtual std::string type() {return "Object";}  ///<Returns the object's type name

        virtual int push(lua_State *l)
        {
            luabridge::push(l,this);
            return 1;
        }///<Adds the object to a given lua state
        static void RegisterLua(lua_State *l)
        {

            GLOBAL::addRegister(Object::TypeID(),l);
            luabridge::getGlobalNamespace(l).beginClass<Object>(Object::TypeID().c_str())
                                            .addConstructor<void (*)(std::string)>()
                                            .addProperty("Name", &Object::getName,&Object::setName)
                                            .addCFunction("getChildren",&Object::getChildArrayLua)
                                            .addProperty("Parent", (Object& (Object::*)() const)&Object::getParentLua,&Object::setParent)
                                            .addCFunction("getParent",&Object::getParentLua)
                                            .addCFunction("findFirstChild",&Object::findFirstChildLua)
                                            .addFunction("isA",&Object::isType)
                                            .addFunction("__eq",(bool (Object::*) (const Object*) const)&Object::luaIsEqual)
                                            .addFunction("type",&Object::type)
                                        .endClass();

        } ///<Adds the class definition to a given lua state

};

#endif // OBJECT_H_INCLUDED
