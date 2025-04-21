cmake_minimum_required(VERSION 3.10)
project(opengl-toy)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include your headers
include_directories(includes)

# Collect sources
file(GLOB SRC_FILES src/*.cpp)

# Add shaders
file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR}/build)

# Add assets
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR}/build)

# Output paths
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

# Add executable
add_executable(${PROJECT_NAME}
    main.cpp
    ${SRC_FILES}
    "src/glad.c"
)

# --- Link GLFW and OpenGL ---
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw OpenGL::GL)
