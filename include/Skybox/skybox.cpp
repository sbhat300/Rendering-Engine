#include "skybox.h"

#include <glad/glad.h>
#include <TGA/tgaLoader.h>
#include <FileLoader/fileLoader.h>
#include <SharedVAO/sharedData.h>

skybox::skybox()
{
    loaded = false;
}

void skybox::load(const char* right, const char* left, const char* top, const char* bottom, const char* front, const char* back)
{
    loaded = true;
    const char* inp[6] = {right, left, top, bottom, front, back};
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    tgaLoader imgLoader;
    for(int i = 0; i < 6; i++)
    {
        imgLoader.load(fileLoader::loadSkybox(*(inp + i), sharedData::windows, "tga").c_str(), false);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, imgLoader.width, imgLoader.height, 0, GL_RGB, GL_UNSIGNED_BYTE, imgLoader.data);
        imgLoader.freeData();
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void skybox::setShader(Shader* s)
{
    shader = s;
    s->use();
    unsigned int matPos = glGetUniformBlockIndex(shader->ID, "Matrices");   
    glUniformBlockBinding(shader->ID, matPos, 0);
    s->setInt("skybox", 0);
}

void skybox::render()
{
    shader->use();
    glDepthMask(GL_FALSE);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    glBindVertexArray(sharedData::skyboxVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);
}