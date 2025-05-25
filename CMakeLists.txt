cmake_minimum_required(VERSION 3.10)
project(Proj2)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG") # Optimize for release

# Find required packages
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

set(SOURCES
    # src/algebra.cpp
    src/main.cpp
    src/camera.cpp
    src/lights.cpp
    src/material.cpp
    # src/mesh.cpp
    src/obj.c
    src/sceneLoader.cpp
    src/sceneObj.cpp
    src/shaders.cpp
    src/locCache.cpp
)


include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/models)

# Add executable target
add_executable(main ${SOURCES})

# Link libraries (GL, GLEW, GLUT)
target_link_libraries(main 
    ${OPENGL_LIBRARIES}
    GLEW
    glut
)
