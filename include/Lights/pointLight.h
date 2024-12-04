#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include <glm/glm.hpp>
#include <Shader/shader.h>

class pointLight
{
    public:
        bool shouldRender;
        glm::vec3 pos;
        glm::vec3 scale;
        Shader* shader;
        float constant, linear, quadratic;
        glm::vec3 diffuseColor, specularColor;
        pointLight(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float c = 1.0f, float l = 0.7f, 
        float q = 1.8f, glm::vec3 diffuseCol = glm::vec3(0.5f, 0.5f, 0.5f), 
        glm::vec3 specCol = glm::vec3(1.0f, 1.0f, 1.0f));
        void render();
        void setShader(Shader* s);
    private:
};

#endif