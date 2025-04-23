#pragma once
#include <glm/ext/matrix_float4x4.hpp>

class Renderable {
public:
    virtual void render(
        glm::mat4 &projection,
        glm::mat4 &view,
        float deltaTime
    );
};
