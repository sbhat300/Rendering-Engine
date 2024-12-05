#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <ModelLoader/mesh.h>
#include <Shader/shader.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <Objects/renderable.h>

class model : public renderable
{
    public:
        std::vector<texture> textures_loaded; 
        std::vector<mesh> meshes;
        float shininess;
        Shader* shader;
        model(std::string path, Shader* s);
        void render();
    private:
        // model data
        std::string directory;
        
        void loadModel(std::string path);
        void processNode(aiNode *node, const aiScene *scene);
        
        mesh processMesh(aiMesh *objMesh, const aiScene *scene);
        std::vector<texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
        unsigned int textureFromFile(const char *path, const std::string &directory, bool srgb);
};

#endif