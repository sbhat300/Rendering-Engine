#include "camera/flyCam.h"

flyCam::flyCam(glm::vec3 position, glm::vec3 up, float camYaw, float camPitch, float sensitivity, float camSpeed, float camFov)
{
    camPos = position;
    worldUp = up;
    yaw = camYaw;
    pitch = camPitch;
    sens = sensitivity;
    speed = camSpeed;
    fov = camFov;
    UpdateCameraVectors();
}

glm::mat4 flyCam::GetViewMatrix()
{
    return glm::lookAt(camPos, camPos + camFront, camUp);
}

void flyCam::ProcessMouseScroll(float yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 90.0f)
        fov = 90.0f;
}

void flyCam::ProcessMouseMovement(float xoffset, float yoffset)
{
    xoffset *= sens;
    yoffset *= sens;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
    yaw = fmod(yaw, (float)360);

    UpdateCameraVectors();
}

void flyCam::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = speed * deltaTime;
    if (direction == FORWARD)
        camPos += camFront * velocity;
    if (direction == BACKWARD)
        camPos -= camFront * velocity;
    if (direction == LEFT)
        camPos -= camRight * velocity;
    if (direction == RIGHT)
        camPos += camRight * velocity;
    if (direction == DOWN)
        camPos -= worldUp * velocity;
    if (direction == UP)
        camPos += worldUp * velocity;
}

void flyCam::UpdateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camFront = glm::normalize(front);
    camRight = glm::normalize(glm::cross(camFront, worldUp));
    camUp = glm::normalize(glm::cross(camRight, camFront));
}