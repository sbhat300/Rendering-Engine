#ifndef SCREEN_H
#define SCREEN_H

#include <Shader/shader.h>

class screen
{
    public:
        int width, height;
        Shader inversionShader;
        Shader blackWhiteShader;
        Shader sharpenShader;
        Shader blurShader;
        Shader* shader;
        unsigned int quadVAO;
        unsigned int quadVBO;
        unsigned int colorBuffer;
        unsigned int fbo;
        unsigned int rbo;
        unsigned int intermediateFbo;
        unsigned int intermediateColorBuffer;
        unsigned int pingPongFbo;
        unsigned int pingPongColorBuffer;
        bool postProcessing[4]; //0 = inversion, 1 = greyscale, 2 = sharpen, 3 = blur
        const float quadVertices[24] = {  
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,

        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f,
        -1.0f,  1.0f,  0.0f, 1.0f
        };	
        void render();
        void addInversion();
        void addBlackWhite();
        void addSharpen();
        void addBlur();
        void setupAntiAliasing(int samples);
        void setShader(Shader* s);
    private:
};

#endif