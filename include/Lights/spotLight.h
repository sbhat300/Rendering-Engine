#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include <glm/glm.hpp>

class spotLight
{
    public:
        glm::vec3 diffuse;
        glm::vec3 specular;
        float cutoff, outerCutoff;
        glm::vec3 position;
        glm::vec3 direction;
        float constant, linear, quadratic;
        spotLight(glm::vec3 d = glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3 spec = glm::vec3(1.0f, 1.0f, 1.0f),
                float co = glm::cos(glm::radians(12.5f)), float oco = glm::cos(glm::radians(17.5f)),
                glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 dir = glm::vec3(0.0f, 0.0f, 1.0f),
                float c = 1.0f, float lin = 0.7f, float qd = 1.8f);
    private:
};

#endif