#pragma once
#include <core/Renderer/Renderable.hpp>
#include <core/shader.hpp>
#include <GLFW/glfw3.h>
#include <glm/ext/vector_float3.hpp>
#include <unordered_map>

enum SceneObjectType {
    SIMPLE_CUBE,
    LIGHT_SOURCE,
};

struct MeshData {
    unsigned int verticesToDraw;
    GLuint VAO;
};

class SceneObject : public Renderable {
private:
    std::string uuod;

    SceneObjectType type;
    glm::vec3 position;
    glm::vec3 eulerRotation;

    Shader *shaderProgram;
    MeshData meshData;

    std::unordered_map<std::string, int> shaderInts;
    std::unordered_map<std::string, int> shaderFloats;
public:
    SceneObject(
        SceneObjectType type,
        Shader *shaderProgram,
        MeshData meshData
    );

    SceneObject(const SceneObject& other);

    void render(
        glm::mat4 &projection,
        glm::mat4 &view
    );

    void setPosition(glm::vec3 position);
    void setEulerRotation(glm::vec3 rotation);
    void addTexture(std::string name, int id);
    glm::vec3 getPosition() const;
    glm::vec3 getEulerRotation() const;
    std::string& getUUOD();

    void setShaderInt(const std::string name, int value);
    void setShaderFloat(const std::string name, float value);

    void logStateDebug();
};
