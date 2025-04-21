#include <camera.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Camera::Camera(float fov, uint32_t FLAGS, CameraType cameraType) {
    this->FLAGS = FLAGS;
    this->fov = fov;
    this->cameraType = cameraType;
    cameraPos = glm::vec3(0.0f, 0.0f,  3.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraUp = glm::vec3(0.0f, 1.0f,  0.0f);
}

glm::mat4 Camera::getLookAtMatrix() {
    return glm::lookAt(this->cameraPos, this->cameraPos + this->cameraFront, this->cameraUp);
}

void Camera::handleCameraRotation() {
    if (FLAGS & Camera::CLAMP_ROTATION) {
        if(pitch > 89.0f)
          pitch =  89.0f;
        if(pitch < -89.0f)
          pitch = -89.0f;
    }

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

void Camera::moveByRelative(float frontDelta, float upDelta, float rightDelta) {
    glm::vec3 normalizedFrontVector = cameraFront;
    if (cameraType == CameraType::FPS_CAMERA) {
        normalizedFrontVector.y = 0.0f;
        normalizedFrontVector = glm::normalize(normalizedFrontVector);
    }
    cameraPos += normalizedFrontVector * frontDelta;
    cameraPos += glm::normalize(glm::cross(normalizedFrontVector, cameraUp)) * rightDelta;
    // std::cout << "[Camera] Moved camera to " << cameraPos.x << ", " << cameraPos.z << ", " << cameraPos.y << std::endl;
};

void Camera::rotateCameraBy(float pitchDelta, float yawDelta) {
    pitch += pitchDelta;
    yaw += yawDelta;
    handleCameraRotation();
};

void Camera::setCameraRotation(float pitch, float yaw) {
    this->pitch = pitch;
    this->yaw = yaw;
    handleCameraRotation();
};

float Camera::getFov() {
    return fov;
};
