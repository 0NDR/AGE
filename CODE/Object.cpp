#include "Object.h"
Object::Object()
{
    setName("DEFAULT");
    setParent(this);
    addNewType();
}
Object::Object(Object* parent)
{
    setName("DEFAULT");
    setParent(parent);
    addNewType();
}
Object::Object(std::string name)
{
    setName(name);
    setParent(this);
    addNewType();
}
Object::Object(Object* parent, std::string name)
{
    setName(name);
    setParent(parent);
    addNewType();
}

bool Object::isType(std::string other)
{
    for(int i=0;i<parenttypes.size();i++)
    {
        if(other == parenttypes[i])
            return true;
    }
    return false;
}

void Object::setParent(Object* newParent)
{
    if(getParent()!= this)
    {
        getParent()->removeChild(this);
    }
    if(newParent != this&& newParent != NULL)
    {
        newParent->addChild(this);
    }
    if(newParent != NULL)
    {
        Parent = newParent;
    }
}

void Object::setName(std::string newName)
{
    Name=newName;
}

std::string Object::getName() const
{
    return Name;
}
int Object::getParentLua(lua_State *l)
{
    return getParent()->push(l);
}
Object* Object::getParent()
{
    if(Parent!=NULL)
    {
        return Parent;
    }
    return this;
}

void Object::addChild(Object* newChild)
{
    children.push_back(newChild);
}

Object* Object::findFirstChild(std::string childName)
{
   for(int i=0;i<children.size();i++)
   {
       if(children[i]->getName() == childName)
            return children[i];
   }
   return NULL;
}

void Object::removeChild(Object* newChild)
{
    for(int i=0;i<children.size();i++)
    {
        if(children[i]==newChild)
        {
            delete children[i];
            children.erase(children.begin()+i);

        }
    }
}

std::vector<Object*> Object::getChildArray()
{
    return children;
}

int Object::getChildArrayLua(lua_State *k)
{
    luabridge::LuaRef r = luabridge::newTable(k);
    for(int i=0;i<children.size();i++)
    {
        (children[i])->push(k);
        r.append(luabridge::Stack<luabridge::LuaRef>::get(k,-1));
    }
    luabridge::push(k, r);
    return 1;
}
int Object::findFirstChildLua(lua_State *k)
{
   bool found = false;
   std::string childName = lua_tostring(k,-1);
   for(int i=0;i<children.size()&&!found;i++)
   {
       if(children[i]->getName() == childName)
       {
            children[i]->push(k);
            found = true;
       }
   }
   return found?1:0;
}
bool Object::luaIsEqual(Object *other)
{
    std::cout<<this<<" x "<<other<<std::endl;
    return this==other;
}
