#include "SceneObject.hpp"
#include <core/shader.hpp>

class SceneObjectBuilder {
private:
    SceneObject *builtObject;
    Shader *lightSourceShaderProgram;
    Shader *simpleCubeShaderProgram;
public:
    SceneObjectBuilder(
        Shader *lightSourceShaderProgram,
        Shader *simpleCubeShaderProgram
    );

    SceneObjectBuilder *makeLighSource();
    SceneObjectBuilder *makeSimpleCube();
    SceneObjectBuilder *setShaderInt(std::string name, int value);
    SceneObjectBuilder *setShaderFloat(std::string name, float value);
    SceneObject* build();
};
