#include "renderableManager.h"
#include <map>
#include <glm/gtx/norm.hpp>
#include <glad/glad.h>

renderableManager::renderableManager(flyCam* cam)
{
    camera = cam;
}
void renderableManager::addObject(renderable* obj, bool translucent)
{
    if(translucent) translucentObjects.push_back(obj);
    else opaqueObjects.push_back(obj);
    obj->translucent = translucent;
}

void renderableManager::renderObjects()
{
    for(renderable* r : opaqueObjects) r->render();
    std::map<float, renderable*> objs;
    for(renderable* r : translucentObjects) objs[glm::length2(camera->camPos - r->pos)] = r;
    for(std::map<float, renderable*>::reverse_iterator r = objs.rbegin(); r != objs.rend(); ++r) r->second->render();
}