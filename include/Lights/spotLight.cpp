#include <Lights/spotLight.h>

spotLight::spotLight(glm::vec3 d, glm::vec3 spec, float co, float oco, glm::vec3 pos, glm::vec3 dir,
              float c, float lin, float qd)
{
    diffuse = d;
    specular = spec;
    cutoff = co;
    outerCutoff = oco;
    position = pos;
    direction = dir;
    constant = c;
    linear = lin;
    quadratic = qd;
}