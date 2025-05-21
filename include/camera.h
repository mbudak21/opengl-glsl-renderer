#ifndef _CAMERA_H
#define _CAMERA_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
private:
    void rotateAroundAxis(const glm::vec3& axis, float angle);
public:
    // Constructors
    Camera();

    // Camera state
    glm::vec3 position;
    glm::vec3 viewDirection; // normalized
    glm::vec3 upDirection;   // normalized
    double fov;
    double nearPlane;
    double farPlane;

    void setRotation(const glm::vec3& rot);

    // Movement methods
    void moveForward(float delta);
    void moveRight(float delta);
    void moveUp(float delta);

    // Rotation methods
    void yaw(float angle);
    void pitch(float angle);
    void roll(float angle);

    // Matrix generation
    glm::mat4 getViewMatrix() const;
    glm::mat4 getPerspectiveMatrix(int screenWidth, int screenHeight) const;
    glm::mat4 getOrthographicMatrix(float left, float right, float bottom, float top) const;
};


#endif