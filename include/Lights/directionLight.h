#ifndef DIRECTIONLIGHT_H
#define DIRECTIONLIGHT_H

#include <glm/glm.hpp>

class directionLight
{
    public:
        glm::vec3 dir;
        glm::vec3 diffuse;
        glm::vec3 specular;
        directionLight(glm::vec3 d = glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3 diff = glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3 spec = glm::vec3(1.0f, 1.0f, 1.0f));
    private:
};

#endif