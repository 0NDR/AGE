#include "LuaScript.h"
int runThread(void* ptr);
int luaWait(lua_State* l){
    luabridge::LuaRef arg = luabridge::LuaRef::fromStack(l,2);
    if(arg.isNumber())
        SDL_Delay(arg.cast<int>());
    else
        lua_yield(l,0);
    return 0;
}
long luaClock(){
    return (long)SDL_GetTicks();
}
struct vec2help: public glm::vec2{
    glm::vec2 operator+(glm::vec2 &other)
    {
        return glm::vec2(x+other.x,y+other.y);
    }
    glm::vec2 operator-(glm::vec2 &other)
    {
        return glm::vec2(x-other.x,y-other.y);
    }
    glm::vec2 operator*(float other)
    {
        return glm::vec2(x*other,y*other);
    }
    glm::vec2 power(float other)
    {
        return glm::vec2(pow(x,other),pow(y,other));
    }
    glm::vec2 operator/(float other)
    {
        return glm::vec2(x/other,y/other);
    }
    bool operator==(glm::vec2 &other)
    {
        return x==other.x&&y==other.y;
    }
    float length()
    {
        return glm::length(glm::vec2(x,y));
    }
    glm::vec2 Normalize()
    {
        glm::vec2 other = glm::normalize(glm::vec2(x,y));
        x=other.x;
        y=other.y;
        return glm::vec2(x,y);
    }
    std::string getString()
    {
        std::stringstream ss;
        ss<<x<<" x "<<y;
        return ss.str();
    }
};
struct vec3help: public glm::vec3{
    glm::vec3 operator+(glm::vec3 &other)
    {
        return glm::vec3(x+other.x,y+other.y,z+other.z);
    }
    glm::vec3 operator-(glm::vec3 &other)
    {
        return glm::vec3(x-other.x,y-other.y,z-other.z);
    }
    glm::vec3 operator*(float other)
    {
        return glm::vec3(x*other,y*other,z*other);
    }
    glm::vec3 power(float other)
    {
        return glm::vec3(pow(x,other),pow(y,other),pow(z,other));
    }
    glm::vec3 operator/(float other)
    {
        return glm::vec3(x/other,y/other,z/other);
    }
    float length()
    {
        return glm::length(glm::vec3(x,y,z));
    }
    glm::vec3 Normalize()
    {
        glm::vec3 other = glm::normalize(glm::vec3(x,y,z));
        x=other.x;
        y=other.y;
        z=other.z;
        return glm::vec3(x,y,z);
    }
    bool operator==(glm::vec3 &other)
    {
        return x==other.x&&y==other.y&&z==other.z;
    }
    std::string getString()
    {
        std::stringstream ss;
        ss<<x<<" x "<<y<<" x "<<z;
        return ss.str();
    }
};
struct vec4help: public glm::vec4{
    glm::vec4 operator+(glm::vec4 &other)
    {
        return glm::vec4(x+other.x,y+other.y,z+other.z,w+other.w);
    }
    glm::vec4 operator-(glm::vec4 &other)
    {
        return glm::vec4(x-other.x,y-other.y,z-other.z,w-other.w);
    }
    glm::vec4 operator*(float other)
    {
        return glm::vec4(x*other,y*other,z*other,w*other);
    }
    glm::vec4 power(float other)
    {
        return glm::vec4(pow(x,other),pow(y,other),pow(z,other),pow(w,other));
    }
    glm::vec4 operator/(float other)
    {
        return glm::vec4(x/other,y/other,z/other,w/other);
    }
    bool operator==(glm::vec4 &other)
    {
        return x==other.x&&y==other.y&&z==other.z&&w==other.w;
    }
    float length()
    {
        return glm::length(glm::vec3(x,y,z));
    }
    glm::vec4 Normalize()
    {
        glm::vec4 other = glm::normalize(glm::vec4(x,y,z,w));
        x=other.x;
        y=other.y;
        z=other.z;
        w=other.w;
        return glm::vec4(x,y,z,w);
    }
    std::string getString()
    {
        std::stringstream ss;
        ss<<x<<" x "<<y<<" x "<<z<<" x "<<w;
        return ss.str();
    }
};
void LuaScript::setState(lua_State* state)
{
    L=lua_newthread(state);
}
lua_State *LuaScript::getState()
{
    if(L==NULL)
    {
        L=luaL_newstate();
        luaopen_io(L);
        luaopen_base(L);
        luaopen_table(L);
        luaopen_string(L);
        luaopen_math(L);
        luaL_openlibs(L);
    }
    return L;
}

void LuaScript::loadFromFile(std::string path)
{
    if(luaL_loadfile(getState(),path.c_str()))
        std::cout<<lua_tostring(getState(),-1);
    isFile = true;
    source = path;
}

void LuaScript::loadString(std::string s)
{
    luaL_loadstring(getState(),s.c_str());
    source =s;
    isFile = false;
}

std::string LuaScript::Run(bool runmultithreaded)
{
    status="";
    multithreaded = runmultithreaded;
    //if(multithreaded)
        //luathread = SDL_CreateThread(runThread,"LuaThread ",this);
   // else
    lua_resume(getState(),NULL,0);
    std::cout<<lua_tostring(getState(),-1)<<std::endl;
    return status;
}
void LuaScript::Continue()
{
    lua_resume(getState(),NULL,1);
}
void LuaScript::Stop()
{
    lua_yield(getState(),0);
}
int cont(lua_State *L)
{
    return 0;
}

int runThread(void* ptr)
{
    LuaScript *theScript = (LuaScript*)ptr;
    luabridge::setGlobal(theScript->getState(),theScript,"this");

    /*if(theScript->isFile)
    {
        if(luaL_dofile(theScript->getState(), theScript->source.c_str()))
            theScript->status = lua_tostring(theScript->getState(),-1);
    }
    else
    {
        if(luaL_dostring(theScript->getState(), theScript->source.c_str()))
            theScript->status =  lua_tostring(theScript->getState(),-1);
    }
    std::cout<<theScript->status;*/
}

void LuaScript::RegisterLua(lua_State *l)
{

        if(!GLOBAL::isRegistered(Resource::TypeID(),l))
        {
            Resource::RegisterLua(l);
        }
        GLOBAL::addRegister(LuaScript::TypeID(),l);
        luabridge::getGlobalNamespace(l).deriveClass<LuaScript,Resource>(TypeID().c_str())
                                                    .addConstructor<void (*)(std::string)>()
                                                    .addFunction("loadString",&LuaScript::loadString)
                                                    .addFunction("Run",&LuaScript::Run)
                                                    .addFunction("__eq",(bool (LuaScript::*) (const LuaScript*) const)&Object::luaIsEqual)
                                        .endClass()
                                        .addCFunction("wait",luaWait)
                                        .addFunction("Clock",luaClock)
                                        .beginClass<glm::vec2>("vec2")
                                            .addConstructor<void (*)(float, float)>()
                                            .addData("x",&vec2help::x)
                                            .addData("y",&vec2help::y)
                                            .addFunction("normalize",(glm::vec2  (glm::vec2 ::*) () const )&vec2help::Normalize)
                                            .addFunction("length",(float  (glm::vec2 ::*) () const )&vec2help::length)
                                            .addFunction("__add",(glm::vec2  (glm::vec2 ::*) (const glm::vec2 &) const )&vec2help::operator+)
                                            .addFunction("__sub",(glm::vec2  (glm::vec2 ::*) (const glm::vec2 &) const )&vec2help::operator-)
                                            .addFunction("__mul",(glm::vec2  (glm::vec2 ::*) (const float) const )&vec2help::operator*)
                                            .addFunction("__div",(glm::vec2  (glm::vec2 ::*) (const float) const )&vec2help::operator/)
                                            .addFunction("__pow",(glm::vec2  (glm::vec2 ::*) (const float) const )&vec2help::power)
                                            .addFunction("__eq",(bool (glm::vec2 ::*) (const glm::vec2 &) const )&vec2help::operator==)
                                            .addFunction("__tostring",(std::string  (glm::vec2 ::*) () const )&vec2help::getString)
                                        .endClass()
                                        .beginClass<glm::vec3>("vec3")
                                            .addConstructor<void (*)(float, float, float)>()
                                            .addData("x",&vec3help::x)
                                            .addData("y",&vec3help::y)
                                            .addData("z",&vec3help::z)
                                            .addData("r",&vec3help::x)
                                            .addData("g",&vec3help::y)
                                            .addData("b",&vec3help::z)
                                            .addFunction("normalize",(glm::vec3  (glm::vec3 ::*) () const )&vec3help::Normalize)
                                            .addFunction("length",(float  (glm::vec3 ::*) () const )&vec3help::length)
                                            .addFunction("__add",(glm::vec3  (glm::vec3 ::*) (const glm::vec3 &) const )&vec3help::operator+)
                                            .addFunction("__sub",(glm::vec3  (glm::vec3 ::*) (const glm::vec3 &) const )&vec3help::operator-)
                                            .addFunction("__mul",(glm::vec3  (glm::vec3 ::*) (const float) const )&vec3help::operator*)
                                            .addFunction("__div",(glm::vec3  (glm::vec3 ::*) (const float) const )&vec3help::operator/)
                                            .addFunction("__pow",(glm::vec3  (glm::vec3 ::*) (const float) const )&vec3help::power)
                                            .addFunction("__eq",(bool (glm::vec3 ::*) (const glm::vec3 &) const )&vec3help::operator==)
                                            .addFunction("__tostring",(std::string  (glm::vec3 ::*) () const )&vec3help::getString)
                                        .endClass()
                                        .beginClass<glm::vec4>("vec4")
                                            .addConstructor<void (*)(float, float, float, float)>()
                                            .addData("x",&vec4help::x)
                                            .addData("y",&vec4help::y)
                                            .addData("z",&vec4help::z)
                                            .addData("w",&vec4help::w)
                                            .addData("r",&vec4help::x)
                                            .addData("g",&vec4help::y)
                                            .addData("b",&vec4help::z)
                                            .addData("a",&vec4help::w)
                                            .addFunction("normalize",(glm::vec4  (glm::vec4 ::*) () const )&vec4help::Normalize)
                                            .addFunction("length",(float  (glm::vec4 ::*) () const )&vec4help::length)
                                            .addFunction("__add",(glm::vec4  (glm::vec4 ::*) (const glm::vec4 &) const )&vec4help::operator+)
                                            .addFunction("__sub",(glm::vec4  (glm::vec4 ::*) (const glm::vec4 &) const )&vec4help::operator-)
                                            .addFunction("__mul",(glm::vec4  (glm::vec4 ::*) (const float) const )&vec4help::operator*)
                                            .addFunction("__div",(glm::vec4  (glm::vec4 ::*) (const float) const )&vec4help::operator/)
                                            .addFunction("__pow",(glm::vec4  (glm::vec4 ::*) (const float) const )&vec4help::power)
                                            .addFunction("__eq",(bool (glm::vec4 ::*) (const glm::vec4 &) const )&vec4help::operator==)
                                            .addFunction("__tostring",(std::string  (glm::vec4 ::*) () const )&vec4help::getString)
                                        .endClass();
}
