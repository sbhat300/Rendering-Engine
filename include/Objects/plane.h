#ifndef PLANE_H
#define PLANE_H

#include <glm/glm.hpp>
#include <TGA/tgaLoader.h>
#include <Shader/shader.h>

class plane
{
    public:
        Shader* shader;
        glm::vec3 pos;
        glm::vec2 scale;
        float xRot, yRot, zRot;
        glm::vec3 color;
        unsigned int texture;
        unsigned int specTex;
        unsigned int emissionTex;
        tgaLoader imgLoader;
        float shininess;
        plane(glm::vec3 p = glm::vec3(0, 0, 0), glm::vec2 s = glm::vec2(1, 1), float xr = 0, float yr = 0, float zr = 0);
        void render();
        void setCol(float r, float g, float b);
        void setShader(Shader* s, unsigned char defaultDiffuse = 255, unsigned char defaultSpec = 127, unsigned char defaultEmission = 0);
        void setTexture(const char* name);
        void setSpecularTexture(const char* name);
        void setEmissionTexture(const char* name);
    private:

};

#endif