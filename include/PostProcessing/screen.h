#ifndef SCREEN_H
#define SCREEN_H

#include <Shader/shader.h>

class screen
{
    public:
        int width, height;
        Shader* shader;
        unsigned int quadVAO;
        unsigned int quadVBO;
        unsigned int colorBuffer;
        unsigned int fbo;
        unsigned int rbo;
        unsigned int intermediateFbo;
        unsigned int intermediateColorBuffer;
        const float quadVertices[24] = {  
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f
        };	
        void render();
        void setupAntiAliasing(int samples);
        void setShader(Shader* s);
    private:
};

#endif