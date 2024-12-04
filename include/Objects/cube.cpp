#include "cube.h"
#include <glad/glad.h>
#include <FileLoader/fileLoader.h>
#include <SharedVAO/sharedData.h>
#include <setup.h>

cube::cube(glm::vec3 p, glm::vec3 s, float xr, float yr, float zr)
{
    pos = p;
    scale = s;
    xRot = xr;
    yRot = yr;
    zRot = zr;
    shininess = 32;
    color = glm::vec3(1, 1, 1);
}

void cube::setCol(float r, float g, float b)
{
    color = glm::vec3(r, g, b);
}

void cube::setShader(Shader* s, unsigned char defaultDiffuse, unsigned char defaultSpec, unsigned char defaultEmission)
{
    shader = s;
    shader->use();
    unsigned int matPos = glGetUniformBlockIndex(shader->ID, "Matrices");   
    glUniformBlockBinding(shader->ID, matPos, 0);
    shader->setFloat("ambient", setup::ambient);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned char temp[3] = {defaultDiffuse, defaultDiffuse, defaultDiffuse};
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, temp);

    glGenTextures(1, &specTex);
    glBindTexture(GL_TEXTURE_2D, specTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned char temp2[3] = {defaultSpec, defaultSpec, defaultSpec};
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, temp2);

    glGenTextures(1, &emissionTex);
    glBindTexture(GL_TEXTURE_2D, emissionTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned char temp3[3] = {defaultEmission, defaultEmission, defaultEmission};
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, temp3);
}

void cube::setTexture(const char* name)
{
    glDeleteTextures(1, &texture);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    imgLoader.load(fileLoader::loadImg(name, sharedData::windows, "tga").c_str(), true); 
    glTexImage2D(GL_TEXTURE_2D, 0, imgLoader.internalType, imgLoader.width, imgLoader.height, 0, imgLoader.type, GL_UNSIGNED_BYTE, imgLoader.data);
    glGenerateMipmap(GL_TEXTURE_2D);
    imgLoader.freeData();
}

void cube::setSpecularTexture(const char* name)
{
    glDeleteTextures(1, &specTex);
    glGenTextures(1, &specTex);
    glBindTexture(GL_TEXTURE_2D, specTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    imgLoader.load(fileLoader::loadImg(name, sharedData::windows, "tga").c_str()); 
    glTexImage2D(GL_TEXTURE_2D, 0, imgLoader.type, imgLoader.width, imgLoader.height, 0, imgLoader.type, GL_UNSIGNED_BYTE, imgLoader.data);
    glGenerateMipmap(GL_TEXTURE_2D);
    imgLoader.freeData();
}

void cube::setEmissionTexture(const char* name)
{
    glDeleteTextures(1, &emissionTex);
    glGenTextures(1, &emissionTex);
    glBindTexture(GL_TEXTURE_2D, emissionTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    imgLoader.load(fileLoader::loadImg(name, sharedData::windows, "tga").c_str(), true); 
    glTexImage2D(GL_TEXTURE_2D, 0, imgLoader.internalType, imgLoader.width, imgLoader.height, 0, imgLoader.type, GL_UNSIGNED_BYTE, imgLoader.data);
    glGenerateMipmap(GL_TEXTURE_2D);
    imgLoader.freeData();
}

void cube::render()
{
    shader->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    shader->setInt("material.diffuse", 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specTex);
    shader->setInt("material.specular", 1);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, emissionTex);
    shader->setInt("material.emission", 2);
    shader->setFloat("material.shininess", shininess);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, pos);
    model = glm::rotate(model, xRot, glm::vec3(1.0f, 0.0f, 0.0f)); 
    model = glm::rotate(model, yRot, glm::vec3(0.0f, 1.0f, 0.0f));  
    model = glm::rotate(model, zRot, glm::vec3(0.0f, 0.0f, 1.0f));  
    model = glm::scale(model, scale);
    shader->setMat4("model", model);
    shader->setVec3("col", color.x, color.y, color.z);
    glBindVertexArray(sharedData::cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}