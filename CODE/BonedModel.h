#ifndef BONED_MODEL_H_INCLUDED
#define BONED_MODEL_H_INCLUDED
#include <map>
#include "Model.h"
struct BoneInfo
{
    glm::mat4 offset;
    glm::mat4 finalt;
    std::string boneName;
};
class BonedModel: public Model///<Basic class for loading *.obj, *.3ds, *.dae, *.blend, and many more
{
    private:
        aiScene *scene;
        std::string FileDirectory;
        glm::mat4 GlobalTransform;
        std::map<std::string,int> boneMap;
        std::vector<BoneInfo> boneInfos;
        std::vector<glm::mat4>boneTransforms;
        int numBones = 0;
    public:
        BonedModel(){addNewType();}
        BonedModel(Object* parent): Model(parent){addNewType();}
        BonedModel(std::string name): Model(name){addNewType();}
        BonedModel(Object* parent, std::string name): Model(parent,name){addNewType();}

        void loadFromFile(std::string filepath){loadFromFileParameters(filepath);}///<Load the mesh from a file with default parameters
        void loadFromFileParameters(std::string filePath,unsigned int Flags = aiProcess_GenSmoothNormals | aiProcess_Triangulate  |aiProcess_FlipUVs|aiProcess_CalcTangentSpace);///<Load the mesh from a file path, with parameteres
        void loadFromAssimp(aiScene* nscene);
        void processNode(aiNode* node);
        void drawToShader(Shader* shdr);
        void saveToFile(std::string filepath);
        glm::mat4 getTransform(std::string name);
        void setTime(double time, std::string AnimationName);
        void readHeirarchy(float t, aiNode* node, glm::mat4 &TransformMat, aiAnimation *anim);
        glm::vec3 InterpolatedScale(float t, aiNodeAnim* nodeAnimation);
        glm::vec3 InterpolatedPosition(float t, aiNodeAnim* nodeAnimation);
        glm::quat InterpolatedRotation(float t, aiNodeAnim* nodeAnimation);


        static std::string TypeID() {return "BonedModel";}///< Returns BonedModel's class name
        virtual std::string type() {return "BonedModel";}
        static void RegisterLua(lua_State *l)
        {

            if(!GLOBAL::isRegistered(Mesh::TypeID(),l))
            {
                Mesh::RegisterLua(l);
            }
            GLOBAL::addRegister(BonedModel::TypeID(),l);
            luabridge::getGlobalNamespace(l).deriveClass<BonedModel,Resource>(TypeID().c_str())
                                                .addConstructor<void (*)(std::string)>()
                                                .addFunction("loadFromFile",&BonedModel::loadFromFile)
                                            .endClass();
    } ///<Adds the class definition to a given lua state
};


#endif // BONED_MODEL_H_INCLUDED
