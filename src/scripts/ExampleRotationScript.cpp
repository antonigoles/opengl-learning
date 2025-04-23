#include <glm/ext/vector_float3.hpp>
#include <scripts/ExampleRotationScript.hpp>

void ExampleRotationScript::onUpdate(SceneObject &object, float deltaTime) {
    glm::vec3 rot = object.getEulerRotation();
    rot.x += deltaTime * 8.0f;
    object.setEulerRotation(rot);
};
