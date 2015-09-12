#ifndef VALUE_H_INCLUDED
#define VALUE_H_INCLUDED
#include "Object.h"
class IntValue: public Object ///base class for every object which changes it's enviorment
{
    protected:
        int Value;
    public:
    IntValue(){addNewType(); }///<
    IntValue(Object* parent): Object(parent){addNewType();}///<
    IntValue(std::string name): Object(name){addNewType();}///<
    IntValue(Object* parent, std::string name): Object(parent,name){addNewType();}///<

    void setValue(int a)  {Value=a;}
    int  getValue() const {return Value;}
    static std::string TypeID() {return "IntValue";}///<Returns Value's class name
    virtual std::string type() {return "IntValue";}
    virtual int push(lua_State *l)
    {
        luabridge::push(l,this);
        return 1;
    }
    static void RegisterLua(lua_State *l)
    {
            if(!GLOBAL::isRegistered(Object::TypeID(),l))
            {
                Object::RegisterLua(l);
            }
            GLOBAL::addRegister(IntValue::TypeID(),l);
            luabridge::getGlobalNamespace(l).deriveClass<IntValue,Object>(TypeID().c_str())
                                                .addProperty("Value",&IntValue::getValue,&IntValue::setValue)
                                            .endClass();
    }
};
class ObjectValue: public Object ///base class for every object which changes it's enviorment
{
    protected:
        Object* Value;
    public:
    ObjectValue(){addNewType(); }///<
    ObjectValue(Object* parent): Object(parent){addNewType();}///<
    ObjectValue(std::string name): Object(name){addNewType();}///<
    ObjectValue(Object* parent, std::string name): Object(parent,name){addNewType();}///<

    void setValue(Object* a)  {Value=a;}
    int getValuecf(lua_State*l)
    {
        if(Value==NULL)
            return 0;
        return Value->push(l);
    }
    static std::string TypeID() {return "ObjectValue";}///<Returns Value's class name
    virtual std::string type() {return "ObjectValue";}
    virtual int push(lua_State *l)
    {
        luabridge::push(l,this);
        return 1;
    }
    static void RegisterLua(lua_State *l)
    {
            if(!GLOBAL::isRegistered(Object::TypeID(),l))
            {
                Object::RegisterLua(l);
            }
            GLOBAL::addRegister(ObjectValue::TypeID(),l);
            luabridge::getGlobalNamespace(l).deriveClass<ObjectValue,Object>(TypeID().c_str())
                                                .addProperty("Value",(Object& (ObjectValue::*)() const)&ObjectValue::getValuecf,&ObjectValue::setValue)
                                            .endClass();
    }
};

#endif // VALUE_H_INCLUDED
