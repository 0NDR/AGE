#include "ValueClasses.h"
void ObjectValue::setValue(Object* a)
{
    if(a!=NULL)
        Value=a;
}
Object* ObjectValue::getValue()
{
    if(Value==NULL)
        return this;
    return Value;
}
int  ObjectValue::getValueLua(lua_State*l)
{
    if(Value==NULL)
        return 0;
    Value->push(l);
    return 1;
}
