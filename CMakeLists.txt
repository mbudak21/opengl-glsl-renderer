cmake_minimum_required(VERSION 3.10)
project(Proj2)

set(CMAKE_CXX_STANDARD 11)

set(SOURCES
    main.cpp
	algebra.cpp
    camera.cpp
    mesh.cpp
)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

# Add executable target
add_executable(main ${SOURCES})

# Link libraries (GL, GLEW, GLUT)
target_link_libraries(main 
    ${OPENGL_LIBRARIES}
    GLEW
    glut
)
