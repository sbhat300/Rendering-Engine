#ifndef LIGHTMANAGER_H
#define LIGHTMANAGER_H

#include <Lights/pointLight.h>
#include <Lights/directionLight.h>
#include <Lights/spotLight.h>
#include <vector>
#include <Shader/shader.h>
#include <glm/gtc/matrix_transform.hpp>

class lightManager
{
    public:
        std::vector<pointLight*> pointLights;
        std::vector<directionLight*> directionLights;
        std::vector<spotLight*> spotLights;
        void loadLights(Shader* shader);
        void updateSpotLight(Shader* shader, int i);
        void updatePointLight(Shader* shader, int i);
        void updateDirectionLight(Shader* shader, int i);
    private:
};

#endif