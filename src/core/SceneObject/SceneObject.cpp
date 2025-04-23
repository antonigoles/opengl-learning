#include <core/shader.hpp>
#include <GLFW/glfw3.h>
#include <core/SceneObject/SceneObject.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>
#include <stdexcept>
#include <string>
// TODO: Implement shared data or something like that
// cuz now we're going to be creating a lot of matrices for no reason each frame

SceneObject::SceneObject(
    SceneObjectType type,
    Shader *shaderProgram,
    MeshData meshData
) {
    this->type = type;
    this->shaderProgram = shaderProgram;
    if (shaderProgram == nullptr) {
        std::cout << "[WARN/FUTURE FATAL] Object with no shader! \n";
    }
    this->meshData = meshData;
    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->eulerRotation = glm::vec3(0.0f, 0.0f, 0.0f);
    srand(time(NULL));
    this->uuod = "UUOD-" + std::to_string((int) (rand() % 10000000));
};

SceneObject::SceneObject(const SceneObject& other) {
    this->uuod = "UUOD-" + std::to_string((int) (rand() % 10000000));
    this->type = other.type;
    this->position = other.position;
    this->eulerRotation = other.eulerRotation;
    this->shaderProgram = other.shaderProgram;
    this->meshData = other.meshData;
    this->shaderInts = other.shaderInts;
    this->shaderFloats = other.shaderFloats;
    // std::cout << "[DEBUG] Copy constructor called on SceneObject class\n";
};

void SceneObject::render(
    glm::mat4 &projection,
    glm::mat4 &view
) {
    if (this->shaderProgram == nullptr) {
        std::cout << "[FATAL] Trying to render object with no shader at " << this->getUUOD() << "\n";
        throw new std::runtime_error("Somehow we're trying to render an object without a shader!");
    }
    this->shaderProgram->use();
    for (auto p : shaderFloats) this->shaderProgram->setFloat(p.first, p.second);
    for (auto p : shaderInts) this->shaderProgram->setInt(p.first, p.second);

    this->shaderProgram->setMat4x4("projection", projection);
    this->shaderProgram->setMat4x4("view", view);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, this->eulerRotation.z, glm::vec3(0.0f, 0.0f, 1.0f)); // first Z
    model = glm::rotate(model, this->eulerRotation.y, glm::vec3(0.0f, 1.0f, 0.0f)); // then Y
    model = glm::rotate(model, this->eulerRotation.x, glm::vec3(1.0f, 0.0f, 0.0f)); // and X
    // TODO: Implement rotations
    model = glm::translate(model, this->position);
    this->shaderProgram->setMat4x4("model", model);

    glBindVertexArray(meshData.VAO);
    glDrawArrays(GL_TRIANGLES, 0, meshData.verticesToDraw);
};

void SceneObject::setPosition(glm::vec3 position) {
    this->position = position;
};

void SceneObject::setEulerRotation(glm::vec3 rotation) {
    this->eulerRotation = rotation;
};

glm::vec3 SceneObject::getPosition() const {
    return this->position;
};

glm::vec3 SceneObject::getEulerRotation() const {
    return this->eulerRotation;
};

void SceneObject::setShaderInt(std::string name, int value) {
    this->shaderInts[name] = value;
};

void SceneObject::setShaderFloat(const std::string name, float value) {
    this->shaderFloats[name] = value;
};

std::string& SceneObject::getUUOD() {
    return this->uuod;
};


void SceneObject::logStateDebug() {
    std::cout << this->uuod << ": { \n";
    std::cout << "    " << "type: " << type << ",\n";

    std::cout << "    " << "position: ("
        << position.x << ", " << position.y << ", " << position.z << ")" << ",\n";

    std::cout << "    " << "eulerRotation: ("
        << eulerRotation.x << ", " << eulerRotation.y << ", " << eulerRotation.z << ")" << ",\n";

    std::cout << "    " << "meshData: { \n";
    std::cout << "        " << "VAO" << ": " << meshData.VAO << ",\n";
    std::cout << "        " << "verticesToDraw" << ": " << meshData.verticesToDraw << ",\n";
    std::cout << "    " << "}, \n";

    std::cout << "    shaderInts: { \n";
    for (auto p : shaderInts ) {
        std::cout << "        " << p.first << ": " << p.second << ",\n";
    }
    std::cout << "    " << "}, \n";
    std::cout << "    shaderInts: { \n";
    for (auto p : shaderFloats ) {
        std::cout << "        " << p.first << ": " << p.second << ",\n";
    }
    std::cout << "    " << "}, \n";

    std::cout << "}\n";
};
