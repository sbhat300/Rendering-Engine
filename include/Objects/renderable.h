#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <glm/glm.hpp>

class renderable
{
    public:
        glm::vec3 pos;
        glm::vec3 scale;
        float xRot, yRot, zRot;
        glm::vec3 color;
        bool translucent;
        virtual void render();
};

#endif