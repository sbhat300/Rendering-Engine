#include <iostream>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <math.h>
#include <Shader/shader.h>
#include <camera/flyCam.h>
#include <TGA/tgaLoader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "config.h"
#include <FileLoader/fileLoader.h>
#include <SharedVAO/sharedData.h>
#include <Objects/cube.h>
#include <setup.h>
#include <Lights/lightManager.h>
#include <Lights/pointLight.h>
#include <Lights/directionLight.h>
#include <Lights/spotLight.h>
#include <Skybox/skybox.h>
#include <vector>
#include <PostProcessing/screen.h>
#include <Objects/plane.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);
void updateDeltaTime();
void cameraControl(float xOff, float yOff);
void setUBO();
void bufferMatrices();
void setShaderCamPos(Shader* s);

float windowHeight = 600, windowWidth = 800;
float deltaTime = 0.0f;
float lastFrame = 0.0f; 
float lastX = 400, lastY = 300;
bool firstMouse = true;
flyCam cam(glm::vec3(0, 0, 3));
tgaLoader imgLoader;
bool sharedData::windows = true;
float setup::ambient = 0.00f;

lightManager lights;

cube c[9];
plane p;

skybox sky;

screen display;

std::string fileLoader::rootPath = ROOT_DIR;
glm::vec3 cubePositions[] = {
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f),
};

unsigned int matrixUBO;
int initWidth, initHeight;

int main() {
    #ifdef __APPLE__
        sharedData::windows = false;
    #endif
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "BRAUM IS A MOVEMENT PLAYER", NULL, NULL);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);  
    glfwSetScrollCallback(window, scroll_callback);
    glfwGetFramebufferSize(window, &initWidth, &initHeight);
    display.width = initWidth;
    display.height = initHeight;
    glViewport(0, 0, display.width, display.height); 

    sharedData::initCubeVAO();
    sharedData::initSkyboxVAO();
    sharedData::initPlaneVAO();

    //setup all necessary shaders here first before anything
    Shader cubeShader("defaultCubeVertex", "defaultCubeFragment");
    Shader lightShader("lightVertex", "lightFragment");
    Shader skyboxShader("skyboxVertex", "skyboxFragment");
    Shader screenShader("screenVertex", "screenFragment");

    display.setShader(&screenShader);
    //Post procssing options after shader is set
    // display.addInversion();
    // display.addBlackWhite();
    // display.addSharpen();
    // display.addBlur();
    display.addBloom(10);
    display.setBloomThreshold(5);

    pointLight temp(glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, 0.7f, 1.8f, glm::vec3(1.0f, 1.0f, 1.0), glm::vec3(2.0f, 2.0f, 2.0f));
    temp.shouldRender = true;
    temp.scale = glm::vec3(0.1f, 0.1f, 0.1f);
    temp.setShader(&lightShader);
    lights.pointLights.push_back(temp);
    temp.pos = glm::vec3(1.0f, 0.0f, -3.0f);
    temp.diffuseColor = glm::vec3(0.0f, 10.0f, 10.0f);
    temp.specularColor = glm::vec3(0.0f, 20.0f, 20.0f);
    lights.pointLights.push_back(temp);
    directionLight temp2(glm::vec3(-0.2f, -0.5f, 0.0f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.2f, 0.2f, 0.2f));
    lights.directionLights.push_back(temp2);
    spotLight temp3( glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f),
                glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f)),
                glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f),
                1.0f, 0.09f, 0.032f);
    // lights.spotLights.push_back(temp3);
    lights.loadLights(&cubeShader);

    
    for(int i = 1; i < 9; i++)
    {
        c[i].setShader(&cubeShader);
        c[i].setTexture("container2");
        c[i].setSpecularTexture("container2_specular");
        c[i].pos = cubePositions[i];
        c[i].scale = glm::vec3(0.5f, 0.5f, 0.5f);
        c[i].shininess = 32;
    }
    c[0].setShader(&cubeShader, 255, 255, 0);
    c[0].pos = glm::vec3(0, -2, 0);
    c[0].scale = glm::vec3(5, 1, 5);
    c[0].shininess = 16;
    c[4].setEmissionTexture("container_emission");

    p.setShader(&cubeShader);
    // p.setTexture("container");
    p.setCol(1, 1, 1);
    p.pos = glm::vec3(0, 0, 1);
    p.scale = glm::vec3(1, 1, 1);
    p.yRot = glm::radians(180.0f);
    p.shininess = 64;

    sky.load("ocean_right", "ocean_left", "ocean_top", "ocean_bottom", "ocean_front", "ocean_back");
    sky.setShader(&skyboxShader);

    //UBO must be set after shaders are set
    setUBO();

    glEnable(GL_DEPTH_TEST);  
    glEnable(GL_CULL_FACE);  

    //anti aliasing
    glEnable(GL_MULTISAMPLE);  
    display.setupAntiAliasing(4);


    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        bufferMatrices();

        setShaderCamPos(&cubeShader);

        // lights.spotLights[0].position = cam.camPos;
        // lights.spotLights[0].direction = cam.camFront;
        // lights.updateSpotLight(&cubeShader, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, display.fbo);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);  
        if(sky.loaded) sky.render();
        for(int i = 0; i < lights.pointLights.size(); i++)
            if(lights.pointLights[i].shouldRender) lights.pointLights[i].render();
        for(int i = 0; i < 9; i++) c[i].render();
        p.render();
        // p.xRot = glfwGetTime() * 0.5f;
        c[3].yRot = glfwGetTime() * 0.5f;

        display.render();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        updateDeltaTime();
    }
    glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    windowWidth = width;
    windowHeight = height;
}
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    const float cameraSpeed = 5 * deltaTime; 
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cam.ProcessKeyboard(DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        cam.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        display.setExposure(display.exposure - 0.01f);
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        display.setExposure(display.exposure + 0.01f);
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        std::cout << "FPS: " << 1.0f / deltaTime << std::endl;
    
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    cameraControl(xpos - lastX, lastY - ypos);
    lastX = xpos;
    lastY = ypos;
}
void cameraControl(float xOff, float yOff) 
{
    cam.ProcessMouseMovement(xOff, yOff);
}
void updateDeltaTime()
{
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;  
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    // cam.ProcessMouseScroll(yoffset);
}

void setUBO()
{
    glGenBuffers(1, &matrixUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, matrixUBO, 0, 2 * sizeof(glm::mat4));
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(cam.fov), display.width / (float)display.height, 0.1f, 100.0f);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
void bufferMatrices()
{
    glm::mat4 view = cam.GetViewMatrix();
    glBindBuffer(GL_UNIFORM_BUFFER, matrixUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
void setShaderCamPos(Shader* s)
{
    s->use();
    s->setVec3("viewPos", cam.camPos.x, cam.camPos.y, cam.camPos.z);
}