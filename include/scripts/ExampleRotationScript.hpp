#include <core/SceneObject/SceneObjectBehaviour.hpp>

class ExampleRotationScript : public SceneObjectBehaviour {
public:
    void onUpdate(SceneObject &object, float deltaTime);
};
