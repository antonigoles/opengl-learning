#include <core/SceneObject/SceneObject.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_common.hpp>
#include <glm/geometric.hpp>
#include <iostream>
#include <core/shader.hpp>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <core/camera.hpp>
#include <core/SceneObject/SceneObjectBuilder.hpp>
#include <vector>

Camera camera(65.0f, Camera::CLAMP_ROTATION, CameraType::FPS_CAMERA);

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

GLuint load_texture(const char* path, int textureUnit, int pixelFormat, bool flipVertical = true) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, texture);
    int width, height, nrChannels;
    if (flipVertical) stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, pixelFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture " << path << std::endl;
    }
    stbi_image_free(data);

    return texture;
}

GLuint load_jpg_texture(const char* path, int textureUnit) {
    return load_texture(path, textureUnit, GL_RGB);
}

GLuint load_png_texture(const char* path, int textureUnit) {
    return load_texture(path, textureUnit, GL_RGBA);
}

GLFWwindow* init_window() {
    std::cout << "[DEBUG] Initializing..." << "\n";
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Open GL Window", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return NULL;
    }

    glViewport(0, 0, 1920, 1080);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    return window;
}

static float currentCameraSpeed = 0.0f;
void processInput(GLFWwindow *window, float deltaTime) {
    const float maxCameraSpeed = 8.5f; // units per second
    const float accelerationTime = 0.2f; // seconds
    const float acceleration = maxCameraSpeed / accelerationTime;
    currentCameraSpeed += acceleration * (deltaTime);
    currentCameraSpeed = std::min(currentCameraSpeed, maxCameraSpeed);

    glm::vec3 movementVector = glm::vec3(0.0f, 0.0f, 0.0f);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        movementVector += glm::vec3(1.0f, 0.0f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        movementVector -= glm::vec3(1.0f, 0.0f, 0.0f);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        movementVector -= glm::vec3(0.0f, 0.0f, 1.0f);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        movementVector += glm::vec3(0.0f, 0.0f, 1.0f);

    if (glm::length(movementVector) > 0.0f) {
        movementVector = glm::normalize(movementVector) * currentCameraSpeed * deltaTime;
        camera.moveByRelative(movementVector.x, movementVector.y, movementVector.z);
    } else {
        currentCameraSpeed = 0.0f;
    }
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static float yaw = -90.0f;
    static float pitch = 0.0f;
    static float lastX = 400, lastY = 300;
    static bool firstMouse = true;
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse=false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.05f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera.rotateCameraBy(yoffset, xoffset);
}

int main() {
    GLFWwindow* window = init_window();
    if(window == NULL) {
        return -1;
    }

    Shader* cubeShader = new Shader(
        "shaders/vertex/vert0.vert",
        "shaders/fragment/frag0.frag"
    );

    Shader* lightingShader = new Shader(
        "shaders/vertex/vert0.vert",
        "shaders/fragment/lightFrag.frag"
    );

    GLuint container_texture = load_jpg_texture("assets/container.jpg", GL_TEXTURE0);
    GLuint awesomeface_texture = load_png_texture("assets/awesomeface.png", GL_TEXTURE1);

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    glfwSetCursorPosCallback(window, mouse_callback);

    SceneObjectBuilder* sceneObjectBuilder = new SceneObjectBuilder(
        lightingShader,
        cubeShader
    );

    std::vector<SceneObject*> sceneObjects;

    sceneObjectBuilder = sceneObjectBuilder
        ->makeSimpleCube()
        ->setShaderInt("texture1", 0)
        ->setShaderInt("texture2", 1);

    for (int i = 0; i<10; i++) {
        sceneObjects.push_back(sceneObjectBuilder->build());
        sceneObjects[i]->setPosition(cubePositions[i]);
        sceneObjects[i]->logStateDebug();
    }

    while(!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, deltaTime);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glEnable(GL_DEPTH_TEST);

        glm::mat4 projection = camera.getProjectionMatrix(window);
        glm::mat4 view = camera.getLookAtMatrix();

        for (int i = 0; i<sceneObjects.size(); i++) {
            sceneObjects[i]->render(projection, view);
        }

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
