#ifndef RENDERABLEMANAGER_H
#define RENDERABLEMANAGER_H

#include <vector>
#include <Objects/renderable.h>
#include <camera/flyCam.h>

class renderableManager
{
    public:
        std::vector<renderable*> translucentObjects;
        std::vector<renderable*> opaqueObjects;
        flyCam* camera;
        renderableManager(flyCam* cam);
        void addObject(renderable* obj, bool translucent);
        void renderObjects();
    private:
};

#endif