#include "model.h"
#define STB_IMAGE_IMPLEMENTATION
#include <Misc/stb_image.h>


model::model(std::string path, Shader* s)
{
    loadModel(path);
    shader = s;
    color = glm::vec3(1.0f, 1.0f, 1.0f);
    pos = glm::vec3(0, 0, 0);
    scale = glm::vec3(1, 1, 1);
    yRot = 0;
    xRot = 0;
    zRot = 0;
}

void model::render()
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, pos);
    model = glm::rotate(model, xRot, glm::vec3(1.0f, 0.0f, 0.0f)); 
    model = glm::rotate(model, yRot, glm::vec3(0.0f, 1.0f, 0.0f));  
    model = glm::rotate(model, zRot, glm::vec3(0.0f, 0.0f, 1.0f));  
    model = glm::scale(model, scale);

    shader->use();
    shader->setMat4("model", model);
    shader->setVec3("col", color.x, color.y, color.z);
    shader->setInt("material.shininess", shininess);
    shader->setInt("material.diffuse", 0);
    shader->setInt("material.specular", 1);
    shader->setInt("material.emission", 2);
    shader->setInt("material.normal", 3);

    if(!translucent) 
    {
        shader->setInt("discardAlpha", 2);
        for(unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].draw();
    }
    else
    {
        glDisable(GL_CULL_FACE);
        glDepthMask(GL_FALSE);
        shader->setInt("discardAlpha", 1);
        for(unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].draw();
        glDepthMask(GL_TRUE);
        shader->setInt("discardAlpha", 0);
        for(unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].draw();
        glEnable(GL_CULL_FACE);
    }
} 

void model::loadModel(std::string path)
{
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);	
    
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
} 

void model::processNode(aiNode *node, const aiScene *scene)
{
    // process all the node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
        meshes.push_back(processMesh(mesh, scene));			
    }
    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}  

mesh model::processMesh(aiMesh *objMesh, const aiScene *scene)
{
    std::vector<vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<texture> textures;

    for(unsigned int i = 0; i < objMesh->mNumVertices; i++)
    {
        vertex v;
        v.position = glm::vec3(objMesh->mVertices[i].x, objMesh->mVertices[i].y, objMesh->mVertices[i].z);
        v.normal = glm::vec3(objMesh->mNormals[i].x, objMesh->mNormals[i].y, objMesh->mNormals[i].z);
        if(objMesh->mTextureCoords[0])
            v.texCoords = glm::vec2(objMesh->mTextureCoords[0][i].x, objMesh->mTextureCoords[0][i].y);
        else
            v.texCoords = glm::vec2(0.0f, 0.0f);  
        v.tangent.x = objMesh->mTangents[i].x;
        v.tangent.y = objMesh->mTangents[i].y;
        v.tangent.z = objMesh->mTangents[i].z;

        vertices.push_back(v);
    }
    // process indices
    for(unsigned int i = 0; i < objMesh->mNumFaces; i++)
    {
        aiFace face = objMesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }  
    // process material
    if(objMesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[objMesh->mMaterialIndex];
        std::vector<texture> diffuseMaps = loadMaterialTextures(material, 
                                            aiTextureType_DIFFUSE, "diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<texture> specularMaps = loadMaterialTextures(material, 
                                            aiTextureType_SPECULAR, "specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        std::vector<texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, 
                                                "normal");  
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        std::vector<texture> emissionMaps = loadMaterialTextures(material, aiTextureType_EMISSIVE, 
                                                "emission");
        textures.insert(textures.end(), emissionMaps.begin(), emissionMaps.end());
    }  

    return mesh(vertices, indices, textures);
}  

std::vector<texture> model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    std::vector<texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for(unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; 
                break;
            }
        }
        if(!skip)
        {   // if texture hasn't been loaded already, load it
            texture tex;
            tex.id = textureFromFile(str.C_Str(), directory, (typeName == "diffuse") || (typeName == "emission"));
            tex.type = typeName;
            tex.path = str.C_Str();
            textures.push_back(tex);
            textures_loaded.push_back(tex); // add to loaded textures
        }
    }
    return textures;
}  

unsigned int model::textureFromFile(const char *path, const std::string &directory, bool srgb)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;        
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        GLenum externFormat = format;
        if(srgb)
        {
            if(externFormat == GL_RGB) externFormat = GL_SRGB;
            else externFormat = GL_SRGB_ALPHA;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, externFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}