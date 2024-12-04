#include "lightManager.h"
#include <string>

void lightManager::loadLights(Shader* shader)
{
    shader->use();
    shader->setInt("numPointLights", pointLights.size());
    for(int i = 0; i < pointLights.size(); i++)
    {        
        shader->setVec3("pointLights[" + std::to_string(i) + "].position", pointLights[i].pos.x, pointLights[i].pos.y, pointLights[i].pos.z);
        shader->setFloat("pointLights[" + std::to_string(i) + "].constant", pointLights[i].constant);
        shader->setFloat("pointLights[" + std::to_string(i) + "].linear", pointLights[i].linear);
        shader->setFloat("pointLights[" + std::to_string(i) + "].quadratic", pointLights[i].quadratic);
        shader->setVec3("pointLights[" + std::to_string(i) + "].diffuse", pointLights[i].diffuseColor.x, pointLights[i].diffuseColor.y, pointLights[i].diffuseColor.z);
        shader->setVec3("pointLights[" + std::to_string(i) + "].specular", pointLights[i].specularColor.x, pointLights[i].specularColor.y, pointLights[i].specularColor.z);
    }
    shader->setInt("numDirLights", directionLights.size());
    for(int i = 0; i < directionLights.size(); i++)
    {
        shader->setVec3("dirLights[" + std::to_string(i) + "].diffuse", directionLights[i].diffuse.x, directionLights[i].diffuse.y, directionLights[i].diffuse.z);
        shader->setVec3("dirLights[" + std::to_string(i) + "].specular", directionLights[i].specular.x, directionLights[i].specular.y, directionLights[i].specular.z); 
        shader->setVec3("dirLights[" + std::to_string(i) + "].direction", directionLights[i].dir.x, directionLights[i].dir.y, directionLights[i].dir.z); 
    }
    shader->setInt("numSpotLights", spotLights.size());
    for(int i = 0; i < spotLights.size(); i++)
    {
        shader->setVec3("spotLights[" + std::to_string(i) + "].position", spotLights[i].position.x, spotLights[i].position.y, spotLights[i].position.z);
        shader->setFloat("spotLights[" + std::to_string(i) + "].constant", spotLights[i].constant);
        shader->setFloat("spotLights[" + std::to_string(i) + "].linear", spotLights[i].linear);
        shader->setFloat("spotLights[" + std::to_string(i) + "].quadratic", spotLights[i].quadratic);
        shader->setVec3("spotLights[" + std::to_string(i) + "].diffuse", spotLights[i].diffuse.x, spotLights[i].diffuse.y, spotLights[i].diffuse.z);
        shader->setVec3("spotLights[" + std::to_string(i) + "].specular", spotLights[i].specular.x, spotLights[i].specular.y, spotLights[i].specular.z);
        shader->setVec3("spotLights[" + std::to_string(i) + "].direction", spotLights[i].direction.x, spotLights[i].direction.y, spotLights[i].direction.z);
        shader->setFloat("spotLights[" + std::to_string(i) + "].cutOff", spotLights[i].cutoff);
        shader->setFloat("spotLights[" + std::to_string(i) + "].outerCutOff", spotLights[i].outerCutoff);
    }
}

void lightManager::updateSpotLight(Shader* shader, int i)
{
    shader->use();
    shader->setVec3("spotLights[" + std::to_string(i) + "].position", spotLights[i].position.x, spotLights[i].position.y, spotLights[i].position.z);
    shader->setFloat("spotLights[" + std::to_string(i) + "].constant", spotLights[i].constant);
    shader->setFloat("spotLights[" + std::to_string(i) + "].linear", spotLights[i].linear);
    shader->setFloat("spotLights[" + std::to_string(i) + "].quadratic", spotLights[i].quadratic);
    shader->setVec3("spotLights[" + std::to_string(i) + "].diffuse", spotLights[i].diffuse.x, spotLights[i].diffuse.y, spotLights[i].diffuse.z);
    shader->setVec3("spotLights[" + std::to_string(i) + "].specular", spotLights[i].specular.x, spotLights[i].specular.y, spotLights[i].specular.z);
    shader->setVec3("spotLights[" + std::to_string(i) + "].direction", spotLights[i].direction.x, spotLights[i].direction.y, spotLights[i].direction.z);
    shader->setFloat("spotLights[" + std::to_string(i) + "].cutOff", spotLights[i].cutoff);
    shader->setFloat("spotLights[" + std::to_string(i) + "].outerCutOff", spotLights[i].outerCutoff);
}

void lightManager::updatePointLight(Shader* shader, int i)
{
    shader->use();
    shader->setVec3("pointLights[" + std::to_string(i) + "].position", pointLights[i].pos.x, pointLights[i].pos.y, pointLights[i].pos.z);
    shader->setFloat("pointLights[" + std::to_string(i) + "].constant", pointLights[i].constant);
    shader->setFloat("pointLights[" + std::to_string(i) + "].linear", pointLights[i].linear);
    shader->setFloat("pointLights[" + std::to_string(i) + "].quadratic", pointLights[i].quadratic);
    shader->setVec3("pointLights[" + std::to_string(i) + "].diffuse", pointLights[i].diffuseColor.x, pointLights[i].diffuseColor.y, pointLights[i].diffuseColor.z);
    shader->setVec3("pointLights[" + std::to_string(i) + "].specular", pointLights[i].specularColor.x, pointLights[i].specularColor.y, pointLights[i].specularColor.z);
}

void lightManager::updateDirectionLight(Shader* shader, int i)
{
    shader->use();
    shader->setVec3("dirLights[" + std::to_string(i) + "].diffuse", directionLights[i].diffuse.x, directionLights[i].diffuse.y, directionLights[i].diffuse.z);
    shader->setVec3("dirLights[" + std::to_string(i) + "].specular", directionLights[i].specular.x, directionLights[i].specular.y, directionLights[i].specular.z); 
    shader->setVec3("dirLights[" + std::to_string(i) + "].direction", directionLights[i].dir.x, directionLights[i].dir.y, directionLights[i].dir.z); 
}