#pragma once
#include <core/SceneObject/SceneObject.hpp>

class SceneObjectBehaviour {
public:
    virtual void onUpdate(SceneObject &object, float deltaTime);
};
