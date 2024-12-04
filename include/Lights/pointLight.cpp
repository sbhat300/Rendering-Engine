#include "pointLight.h"
#include <glad/glad.h>
#include <SharedVAO/sharedData.h>

pointLight::pointLight(glm::vec3 position, float c, float l, float q, glm::vec3 diffuseCol,
glm::vec3 specCol)
{
    pos = position;
    constant = c;
    linear = l;
    quadratic = q;
    diffuseColor = diffuseCol;
    specularColor = specCol;
    shouldRender = false;
}

void pointLight::setShader(Shader* s)
{
    shader = s;
    shader->use();
    unsigned int matPos = glGetUniformBlockIndex(s->ID, "Matrices");   
    glUniformBlockBinding(s->ID, matPos, 0);
}

void pointLight::render()
{
    shader->use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, pos);
    model = glm::scale(model, scale);
    shader->setMat4("model", model);
    shader->setVec3("col", specularColor.x, specularColor.y, specularColor.z);
    glBindVertexArray(sharedData::cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}