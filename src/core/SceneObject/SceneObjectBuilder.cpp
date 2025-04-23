#include <core/SceneObject/SceneObject.hpp>
#include <core/SceneObject/SceneObjectBuilder.hpp>
#include <iostream>
#include <stdexcept>

GLuint getUVMappedCubeVAO() {
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    return VAO;
}

SceneObjectBuilder::SceneObjectBuilder(
    Shader *lightSourceShaderProgram,
    Shader *simpleCubeShaderProgram
) {
    this->lightSourceShaderProgram = lightSourceShaderProgram;
    this->simpleCubeShaderProgram = simpleCubeShaderProgram;

    bool nullShader =
        this->lightSourceShaderProgram == nullptr |
        this->simpleCubeShaderProgram == nullptr;

    if (nullShader) {
        std::cout << "[WARN] Builder with null shader!\n";
    }
};

SceneObjectBuilder* SceneObjectBuilder::makeLighSource() {
    MeshData md;
    md.VAO = getUVMappedCubeVAO();
    md.verticesToDraw = 36;

    this->builtObject = new SceneObject(
        SceneObjectType::LIGHT_SOURCE,
        this->lightSourceShaderProgram,
        md
    );

    return this;
};

SceneObjectBuilder* SceneObjectBuilder::makeSimpleCube() {
    MeshData md;
    md.VAO = getUVMappedCubeVAO();
    md.verticesToDraw = 36;

    this->builtObject = new SceneObject(
        SceneObjectType::SIMPLE_CUBE,
        this->simpleCubeShaderProgram,
        md
    );

    return this;
};

SceneObjectBuilder *SceneObjectBuilder::setShaderInt(std::string name, int value) {
    this->builtObject->setShaderInt(name, value);
    return this;
};

SceneObjectBuilder *SceneObjectBuilder::setShaderFloat(std::string name, float value) {
    this->builtObject->setShaderFloat(name, value);
    return this;
};

SceneObject* SceneObjectBuilder::build() {
    if (this->builtObject == nullptr) {
        throw new std::runtime_error("Nothing has been built with this builder yet");
    }

    // we want this to be a copy
    // nasty engineering smh
    SceneObject *result = new SceneObject(*(this->builtObject));
    return result;
};
