#ifndef CUBE_H
#define CUBE_H

#include <glm/glm.hpp>
#include <Shader/shader.h>
#include <TGA/tgaLoader.h>
#include <Objects/renderable.h>

class cube : public renderable
{
    public:
        Shader* shader;
        unsigned int texture;
        unsigned int specTex;
        unsigned int emissionTex;
        unsigned int normalTex;
        tgaLoader imgLoader;
        float shininess;
        cube(glm::vec3 p = glm::vec3(0, 0, 0), glm::vec3 s = glm::vec3(1, 1, 1), float xr = 0, float yr = 0, float zr = 0);
        void render();
        void setCol(float r, float g, float b);
        void setShader(Shader* s, unsigned char defaultDiffuse = 255, unsigned char defaultSpec = 127, unsigned char defaultEmission = 0);
        void setTexture(const char* name);
        void setSpecularTexture(const char* name);
        void setEmissionTexture(const char* name);
        void setNormalTexture(const char* name);
    private:

};

#endif