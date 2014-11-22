#include "ResourceFactory.h"

Mesh *ResourceFactory::loadMesh(std::string path)
{
    for(int i=0;i<loadedMeshList.size();i++)
    {
        if(loadedMeshList[i].path == path)
        {
            Mesh *m = loadedMeshList[i].o;
            return m;
        }
    }
    Mesh *m = new Mesh();
    m->meshFromFile(path);
    loadedMeshList.push_back(meshHolder(path,m));
    return m;
}

glTexture ResourceFactory::loadTexture(std::string path)
{
    for(int i=0;i<loadedTextureList.size();i++)
    {
        if(loadedTextureList[i].path == path)
            return loadedTextureList[i].o;
    }
    glTexture t;
    t.loadRawFromFile(path);
    loadedTextureList.push_back(textureHolder(path,t));
    return t;
}
