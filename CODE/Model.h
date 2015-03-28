#ifndef MODEL_H_INCLUDED
#define MODEL_H_INCLUDED
#include "Mesh.h"
class Model: public Resource///<Basic class for loading *.obj, *.3ds, *.dae, *.blend, and many more
{
    private:
        aiScene *scene;
        std::string FileDirectory;
    public:
        std::vector<Mesh*> Meshes;///<Vector holding the individual GL buffers for each mesh part

        Model(){addNewType();}
        Model(Object* parent): Resource(parent){addNewType();}
        Model(std::string name): Resource(name){addNewType();}
        Model(Object* parent, std::string name): Resource(parent,name){addNewType();}

        aiScene* getScene();///<Returns the aiScene form of the mesh, holding all the mesh data
        void loadFromFile(std::string filepath){loadFromFileParameters(filepath);}///<Load the mesh from a file with default parameters
        void loadFromFileParameters(std::string filePath,unsigned int Flags = aiProcess_GenSmoothNormals | aiProcess_Triangulate  |aiProcess_FlipUVs|aiProcess_CalcTangentSpace);///<Load the mesh from a file path, with parameteres
        void loadFromAssimp(aiScene* nscene);
        void processNode(aiNode* node);
        virtual void drawToShader(Shader* shdr);
        void saveToFile(std::string filepath);

        static std::string TypeID() {return "Model";}///< Returns Model's class name
        virtual std::string type() {return "Model";}
        static void RegisterLua(lua_State *l)
        {

            if(!GLOBAL::isRegistered(Mesh::TypeID(),l))
            {
                Mesh::RegisterLua(l);
            }
            GLOBAL::addRegister(Model::TypeID(),l);
            luabridge::getGlobalNamespace(l).deriveClass<Model,Resource>(TypeID().c_str())
                                                .addConstructor<void (*)(std::string)>()
                                                .addFunction("loadFromFile",&Model::loadFromFile)
                                            .endClass();
    } ///<Adds the class definition to a given lua state
};
#endif // MODEL_H_INCLUDED
