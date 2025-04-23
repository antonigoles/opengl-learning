#pragma once
#include <core/Renderer/Renderable.hpp>
#include <core/camera.hpp>
#include <vector>

class SceneRenderer {
private:
Camera *camera;
std::vector<Renderable*> renderableObjects;

public:
    SceneRenderer(Camera *camera);

    void registerObject(Renderable *renderableObject);
    void renderAllObjects();
};
