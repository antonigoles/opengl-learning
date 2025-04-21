#pragma once
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>

enum CameraType {
    FREE_CAMERA,
    FPS_CAMERA
};

class Camera {
private:
    CameraType cameraType;
    uint32_t FLAGS = 0;
    float yaw = -90.0f;
    float pitch = 0.0f;
    float fov;
    glm::vec3 cameraPos, cameraFront, cameraUp;

    void handleCameraRotation();
public:
    static const uint32_t CLAMP_ROTATION = 1;

    Camera(float fov, uint32_t FLAGS = 0, CameraType cameraType = FPS_CAMERA);
    glm::mat4 getLookAtMatrix();

    void moveByRelative(float frontDelta, float upDelta, float rightDelta);
    void rotateCameraBy(float pitchDelta, float yawDelta);
    void setCameraRotation(float pitch, float yaw);

    float getFov();
};
