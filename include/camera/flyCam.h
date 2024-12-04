#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <math.h>       

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

//default values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 5.0f;
const float SENSITIVITY = 0.15f;
const float FOV =  45.0f;

class flyCam
{
    public:
        glm::vec3 camPos;
        glm::vec3 camFront;
        glm::vec3 camUp;
        glm::vec3 camRight;
        glm::vec3 worldUp;
        float pitch;
        float yaw;
        float fov;
        float sens;
        float speed;
        flyCam(glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 up = glm::vec3(0, 1, 0), float camYaw = YAW, float camPitch = PITCH, float sensitivity = SENSITIVITY, float camSpeed = SPEED, float camFov = FOV);
        glm::mat4 GetViewMatrix();
        void ProcessMouseScroll(float yoffset);
        void ProcessMouseMovement(float xoffset, float yoffset);
        void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    private:
        void UpdateCameraVectors();
};

#endif