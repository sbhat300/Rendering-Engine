#include "screen.h"

#include <glad/glad.h>
#include <iostream>

void screen::setShader(Shader* s)
{
    shader = s;
    shader->use();
    shader->setInt("screenTexture", 0);
    shader->setInt("blurTexture", 0);
    shader->setInt("bloomEnabled", 0);
    exposure = 1.0f;
    shader->setFloat("exposure", exposure);
    bloomEnabled = false;

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    glGenFramebuffers(1, &intermediateFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, intermediateFbo);
    glGenTextures(1, &intermediateColorBuffer);
    glBindTexture(GL_TEXTURE_2D, intermediateColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, intermediateColorBuffer, 0);

    glGenFramebuffers(1, &pingPongFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, pingPongFbo);
    glGenTextures(1, &pingPongColorBuffer);
    glBindTexture(GL_TEXTURE_2D, pingPongColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingPongColorBuffer, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void screen::render()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFbo);
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glDisable(GL_DEPTH_TEST);

    int currBuffer = intermediateColorBuffer;
    if(postProcessing[0])
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingPongFbo);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
        // glClear(GL_COLOR_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, currBuffer);
        blackWhiteShader.use();
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        currBuffer = pingPongColorBuffer;
    }
    if(postProcessing[1])
    {
        if(currBuffer == intermediateColorBuffer) glBindFramebuffer(GL_FRAMEBUFFER, pingPongFbo);
        else glBindFramebuffer(GL_FRAMEBUFFER, intermediateFbo);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
        // glClear(GL_COLOR_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, currBuffer);
        sharpenShader.use();
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        if(currBuffer == intermediateColorBuffer) currBuffer = pingPongColorBuffer;
        else currBuffer = intermediateColorBuffer;
    }
    if(postProcessing[2])
    {
        if(currBuffer == intermediateColorBuffer) glBindFramebuffer(GL_FRAMEBUFFER, pingPongFbo);
        else glBindFramebuffer(GL_FRAMEBUFFER, intermediateFbo);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
        // glClear(GL_COLOR_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, currBuffer);
        blurShader.use();
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        if(currBuffer == intermediateColorBuffer) currBuffer = pingPongColorBuffer;
        else currBuffer = intermediateColorBuffer;
    }
    unsigned int oldCurr = currBuffer;


    if(bloomEnabled)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, bloomFbo);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, currBuffer);
        bloomShader.use();
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        bloomBlurShader.use();
        bool horizontal = true;
        int pingPongFbos[2];
        pingPongFbos[0] = bloomFbo;
        int pingPongBuffers[2];
        pingPongBuffers[0] = bloomColorBuffer;
        if(currBuffer == intermediateColorBuffer)
        {
            pingPongFbos[1] = pingPongFbo;
            pingPongBuffers[1] = pingPongColorBuffer;
        }
        else
        {
            pingPongFbos[1] = intermediateFbo;
            pingPongBuffers[1] = intermediateColorBuffer;
        }
        horizontal = true;
        glBindVertexArray(quadVAO);
        for(int i = 0; i < bloomStrength; ++i)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, pingPongFbos[horizontal]);
            bloomBlurShader.setInt("horizontal", horizontal);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, pingPongBuffers[!horizontal]);
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
            // glClear(GL_COLOR_BUFFER_BIT);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            horizontal = !horizontal;
        }
        currBuffer = pingPongBuffers[horizontal];
    }

    shader->use();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
    // glClear(GL_COLOR_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, oldCurr);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, currBuffer);
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void screen::setupAntiAliasing(int samples)
{
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glGenTextures(1, &colorBuffer);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, colorBuffer);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB16F, width, height, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, colorBuffer, 0);
    
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT24, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void screen::addBlackWhite()
{
    blackWhiteShader = Shader("screenVertex", "blackWhiteFragment");
    blackWhiteShader.use();
    blackWhiteShader.setInt("screenTexture", 0);
    postProcessing[0] = true;
}

void screen::addSharpen()
{
    sharpenShader = Shader("screenVertex", "sharpenFragment");
    sharpenShader.use();
    sharpenShader.setInt("screenTexture", 0);
    postProcessing[1] = true;
}
void screen::addBlur()
{
    blurShader = Shader("screenVertex", "blurFragment");
    blurShader.use();
    blurShader.setInt("screenTexture", 0);
    postProcessing[2] = true;
}

void screen::setExposure(float exp)
{
    exposure = std::max(exp, 0.0f);
    shader->use();
    shader->setFloat("exposure", exposure);
}

void screen::setBloomThreshold(float t)
{
    bloomThreshold = t;
    bloomShader.use();
    bloomShader.setFloat("threshold", t);
}

void screen::addBloom(int s)
{
    bloomStrength = s;
    bloomShader = Shader("screenVertex", "bloomFragment");
    bloomShader.use();
    bloomShader.setInt("screenTexture", 0);
    bloomShader.setFloat("threshold", 1.0f);

    glGenFramebuffers(1, &bloomFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, bloomFbo);
    glGenTextures(1, &bloomColorBuffer);
    glBindTexture(GL_TEXTURE_2D, bloomColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bloomColorBuffer, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    bloomBlurShader = Shader("screenVertex", "bloomBlurFragment");
    bloomBlurShader.use();
    bloomBlurShader.setInt("screenTexture", 0);

    shader->use();
    shader->setInt("bloomEnabled", 1);
    shader->setInt("blurTexture", 1);
    bloomEnabled = true;
}