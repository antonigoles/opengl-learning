#include <core/Renderer/SceneRenderer.hpp>

SceneRenderer::SceneRenderer(Camera *camera) {
    this->camera = camera;
};

void SceneRenderer::registerObject(Renderable *renderableObject) {
    this->renderableObjects.push_back(renderableObject);
};

void SceneRenderer::renderAllObjects() {
    for (auto obj : this->renderableObjects) {
        // TODO: Implement this
        glm::mat4 projection;
        glm::mat4 view;
        float deltaTime = 0.0f;

        obj->render(projection, view, deltaTime);
    }
};
