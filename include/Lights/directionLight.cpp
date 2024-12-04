#include "directionLight.h"

directionLight::directionLight(glm::vec3 d, glm::vec3 diff, glm::vec3 spec)
{
    dir = d;
    diffuse = diff;
    specular = spec;
}