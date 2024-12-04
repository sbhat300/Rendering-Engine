#ifndef SKYBOX_H
#define SKYBOX_H

#include <vector>
#include <Shader/shader.h>

class skybox
{
    public:
        unsigned int texture;
        Shader* shader;
        bool loaded;
        skybox();
        void load(const char* right, const char* left, const char* top, const char* bottom, const char* front, const char* back);
        void setShader(Shader* s);
        void render();
    private:

};

#endif