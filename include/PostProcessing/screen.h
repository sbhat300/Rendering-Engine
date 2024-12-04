#ifndef SCREEN_H
#define SCREEN_H

#include <Shader/shader.h>

class screen
{
    public:
        int width, height;
        Shader blackWhiteShader;
        Shader sharpenShader;
        Shader blurShader;
        Shader bloomShader;
        Shader bloomBlurShader;
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
        unsigned int bloomFbo;
        unsigned int bloomColorBuffer;
        int bloomStrength;
        bool postProcessing[3]; //0 = greyscale, 1 = sharpen, 2 = blur
        float exposure;
        float bloomThreshold;
        bool bloomEnabled;
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
        void addBlackWhite();
        void addSharpen();
        void addBlur();
        void addBloom(int s);
        void setExposure(float exp);
        void setBloomThreshold(float t);
        void setupAntiAliasing(int samples);
        void setShader(Shader* s);
    private:
};

#endif