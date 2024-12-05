#include "SharedVAO/sharedData.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>

unsigned int sharedData::cubeVAO = 0;
unsigned int sharedData::cubeVBO = 0;
unsigned int sharedData::skyboxVAO = 0;
unsigned int sharedData::skyboxVBO = 0;
unsigned int sharedData::planeVAO = 0;
unsigned int sharedData::planeVBO = 0;
unsigned int sharedData::defaultDiffTex = 0;
unsigned int sharedData::defaultSpecTex = 0;
unsigned int sharedData::defaultEmissionTex = 0;
unsigned int sharedData::defaultNormalTex = 0;

glm::mat3 sharedData::planeTBNInv = glm::mat3(1.0f);

void sharedData::initPlaneVAO()
{
    float planeVertices[66] = {
        // front face
        -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left
        1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
        1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, // top-right
        1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, // top-right
        -1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // top-left
        -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f // bottom-left
    };
    for(int i = 0; i < 2; i++)
    {
        glm::vec3 pos[3];
        glm::vec2 uv[3];
        for(int j = 0; j < 3; j++) 
        {
            pos[j] = glm::vec3(planeVertices[j * 11 + 33 * i], planeVertices[j * 11 + 1 + 33 * i], planeVertices[j * 11 + 2 + 33 * i]);
            uv[j] = glm::vec2(planeVertices[j * 11 + 6 + 33 * i], planeVertices[j * 11 + 7 + 33 * i]);
        }
        glm::vec3 e1 = pos[1] - pos[0];
        glm::vec3 e2 = pos[2] - pos[0];
        glm::vec2 duv1 = uv[1] - uv[0];
        glm::vec2 duv2 = uv[2] - uv[0];
        float det = 1.0f / (duv1.x * duv2.y - duv2.x * duv1.y);
        glm::vec3 tangent = glm::vec3(duv2.y * e1.x - duv1.y * e2.x, 
                                     duv2.y * e1.y - duv1.y * e2.y, 
                                     duv2.y * e1.z - duv1.y * e2.z);
        for(int j = 0; j < 3; j++)
        {
            planeVertices[j * 11 + 33 * i + 8] = tangent.x;
            planeVertices[j * 11 + 33 * i + 9] = tangent.y;
            planeVertices[j * 11 + 33 * i + 10] = tangent.z;
        }
    }
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(0);  
    glEnableVertexAttribArray(1);  
    glEnableVertexAttribArray(2);  
    glEnableVertexAttribArray(3);  
    glBindVertexArray(0); 
}

void sharedData::initCubeVAO()

{
    float cubeVertices[396] = {
        // front face
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left
        1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-right
        1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, // top-right
        1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, // top-right
        -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // top-left
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left

        // back face
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left
        1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, // top-right
        1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-right         
        1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, // top-right
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left
        -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // top-left

        // left face
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // top-right
        -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, // top-left
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // bottom-left
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // bottom-left
        -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-right
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // top-right
        // right face
        1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // top-left
        1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
        1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, // top-right         
        1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
        1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // top-left
        1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left     
        // bottom face
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // top-right
        1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, // top-left
        1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left
        1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left
        -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-right
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // top-right
        // top face
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // top-left
        1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-right
        1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, // top-right     
        1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-right
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // top-left
        -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f // bottom-left   
    };

    for(int i = 0; i < 12; i++)
    {
        glm::vec3 pos[3];
        glm::vec2 uv[3];
        for(int j = 0; j < 3; j++) 
        {
            pos[j] = glm::vec3(cubeVertices[j * 11 + 33 * i], cubeVertices[j * 11 + 1 + 33 * i], cubeVertices[j * 11 + 2 + 33 * i]);
            uv[j] = glm::vec2(cubeVertices[j * 11 + 6 + 33 * i], cubeVertices[j * 11 + 7 + 33 * i]);
        }
        glm::vec3 e1 = pos[1] - pos[0];
        glm::vec3 e2 = pos[2] - pos[0];
        glm::vec2 duv1 = uv[1] - uv[0];
        glm::vec2 duv2 = uv[2] - uv[0];
        float det = 1.0f / (duv1.x * duv2.y - duv2.x * duv1.y);
        glm::vec3 tangent = glm::vec3(duv2.y * e1.x - duv1.y * e2.x, 
                                     duv2.y * e1.y - duv1.y * e2.y, 
                                     duv2.y * e1.z - duv1.y * e2.z);
        for(int j = 0; j < 3; j++)
        {
            cubeVertices[j * 11 + 33 * i + 8] = tangent.x;
            cubeVertices[j * 11 + 33 * i + 9] = tangent.y;
            cubeVertices[j * 11 + 33 * i + 10] = tangent.z;
        }
    }

    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVAO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(0);  
    glEnableVertexAttribArray(1);  
    glEnableVertexAttribArray(2);  
    glEnableVertexAttribArray(3);  
    glBindVertexArray(0); 
}

void sharedData::initSkyboxVAO()
{
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVAO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void sharedData::initDefaultTextures()
{
    glGenTextures(1, &defaultDiffTex);
    glBindTexture(GL_TEXTURE_2D, defaultDiffTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    float temp[3] = {1.0f, 1.0f, 1.0f};
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_FLOAT, temp);

    glGenTextures(1, &defaultSpecTex);
    glBindTexture(GL_TEXTURE_2D, defaultSpecTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    float temp2[3] = {0.5f, 0.5f, 0.5f};
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_FLOAT, temp2);

    glGenTextures(1, &defaultEmissionTex);
    glBindTexture(GL_TEXTURE_2D, defaultEmissionTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    float temp3[3] = {0, 0, 0};
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_FLOAT, temp3);

    glGenTextures(1, &defaultNormalTex);
    glBindTexture(GL_TEXTURE_2D, defaultNormalTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    float temp4[3] = {0.5f, 0.5f, 1.0f};
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_FLOAT, temp4);
}