#ifndef MESH_H
#define MESH_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Shader/shader.h>

#include <string>
#include <vector>

struct vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 tangent;
};

struct texture {
    unsigned int id;
    std::string type;
    std::string path;
};  

class mesh
{
    public:
        std::vector<vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<texture> textures;

        mesh(std::vector<vertex> vertices, std::vector<unsigned int> indices, std::vector<texture> textures);
        void draw();
    private:
        unsigned int VAO, VBO, EBO;
        void setupMesh();
};

#endif