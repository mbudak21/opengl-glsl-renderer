#include "camera.h"

Camera::Camera()
    : position(0.0f, 0.0f, 20.0f),
      viewDirection(0.0f, 0.0f, -1.0f),
      upDirection(0.0f, 1.0f, 0.0f),
      fov(60.0),
      nearPlane(0.1),
      farPlane(1000.0) {}

void Camera::setRotation(const glm::vec3& rot) {
    float alpha = glm::radians(rot.x);
    float beta = glm::radians(rot.y);
    float gamma = glm::radians(rot.z);

    // Rz(-gamma) * Ry(-beta) * Rx(-alpha)
    glm::mat4 Rz = glm::rotate(glm::mat4(1.0f), -gamma, glm::vec3(0, 0, 1));
    glm::mat4 Ry = glm::rotate(glm::mat4(1.0f), -beta,  glm::vec3(0, 1, 0));
    glm::mat4 Rx = glm::rotate(glm::mat4(1.0f), -alpha, glm::vec3(1, 0, 0));
    glm::mat4 R = Rz * Ry * Rx;

    // Update the view and up directions
    viewDirection = glm::normalize(glm::vec3(R * glm::vec4(0, 0, -1, 0)));
    upDirection = glm::normalize(glm::vec3(R * glm::vec4(0, 1,  0, 0)));
}

void Camera::moveForward(float delta) {
    position += delta * viewDirection;
}

void Camera::moveRight(float delta) {
    glm::vec3 right = glm::normalize(glm::cross(viewDirection, upDirection));
    position += delta * right;
}

void Camera::moveUp(float delta) {
    position += delta * upDirection;
}

void Camera::yaw(float angle) {
    rotateAroundAxis(upDirection, angle);
}

void Camera::pitch(float angle) {
    glm::vec3 right = glm::normalize(glm::cross(viewDirection, upDirection));
    rotateAroundAxis(right, angle);
}

void Camera::roll(float angle) {
    rotateAroundAxis(viewDirection, angle);
}

void Camera::rotateAroundAxis(const glm::vec3& axis, float angle) {
    glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);
    viewDirection = glm::normalize(glm::vec3(rot * glm::vec4(viewDirection, 0.0f)));
    upDirection = glm::normalize(glm::vec3(rot * glm::vec4(upDirection, 0.0f)));
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + viewDirection, upDirection);
}

glm::mat4 Camera::getPerspectiveMatrix(int screenWidth, int screenHeight) const {
    return glm::perspective(glm::radians((float)fov), (float)screenWidth / screenHeight, (float)nearPlane, (float)farPlane);
}

glm::mat4 Camera::getOrthographicMatrix(float left, float right, float bottom, float top) const {
    return glm::ortho(left, right, bottom, top, (float)nearPlane, (float)farPlane);
}



// ==== Old Functions Which Used Algebra.h

// Matrix OrthographicProjectionMatrix(float left, float right, float top, float bottom, float near, float far) {
//     Matrix ortho;
//     ortho.e[0]  = 2.f / (right - left);
//     ortho.e[1]  = 0.f;
//     ortho.e[2]  = 0.f;
//     ortho.e[3]  = 0.f;

//     ortho.e[4]  = 0.f;
//     ortho.e[5]  = 2.f / (top - bottom);
//     ortho.e[6]  = 0.f;
//     ortho.e[7]  = 0.f;

//     ortho.e[8]  = 0.f;
//     ortho.e[9]  = 0.f;
//     ortho.e[10] = -2.f / (far - near);
//     ortho.e[11] = 0.f;

//     ortho.e[12] = -(right + left) / (right - left);
//     ortho.e[13] = -(top + bottom) / (top - bottom);
//     ortho.e[14] = -(far + near) / (far - near);
//     ortho.e[15] = 1.f;

//     return ortho;
// }


// Matrix PerspectiveProjectionMatrix(Camera cam, int screenWidth, int screenHeight) {
// 	// P.e[0] = 1.299038f;	P.e[4] = 0.000000f;	P.e[ 8] =  0.000000f;	P.e[12] =  0.0f;
// 	// P.e[1] = 0.000000f;	P.e[5] = 1.732051f;	P.e[ 9] =  0.000000f;	P.e[13] =  0.0f;
// 	// P.e[2] = 0.000000f;	P.e[6] = 0.000000f;	P.e[10] = -1.000200f;	P.e[14] = -2.000200f;
// 	// P.e[3] = 0.000000f;	P.e[7] = 0.000000f;	P.e[11] = -1.000000f;	P.e[15] =  0.0f;

// 	// Assignment 1: Calculate the projection transform yourself 	
// 	// The matrix P should be calculated from camera parameters
// 	// Therefore, you need to replace this hard-coded transform.
// 	Matrix P;

// 	float aspect = (float)screenWidth / (float)screenHeight;
//     float fovY = cam.fov;
//     float near = cam.nearPlane;
//     float far = cam.farPlane;
// 	float f = 1.0f / tanf(fovY / 2.0f);

// 	float a = f / aspect;
// 	float b = f;
// 	float c = -(far + near) / (far - near);
// 	float d = -(2 * far * near) / (far - near);
	
// 	printf("a = %f, b = %f, c = %f, d = %f\n", a, b, c, d);

// 	P.e[0] = a;		P.e[4] = 0.f;	P.e[ 8] = 0.f;	P.e[12] = 0.0f;
// 	P.e[1] = 0.f;	P.e[5] = b;		P.e[ 9] = 0.f;	P.e[13] = 0.0f;
// 	P.e[2] = 0.f;	P.e[6] = 0.f;	P.e[10] = c;	P.e[14] = d;
// 	P.e[3] = 0.f;	P.e[7] = 0.f;	P.e[11] = -1.f;	P.e[15] = 0.0f;

// 	return P;
// }
// //Interface function for view matrix
// Matrix ViewMatrix(Camera cam)
// {
// 	// Assignment 1: Calculate the transform to view coordinates yourself 	
// 	// The matrix V should be calculated from camera parameters
// 	// Therefore, you need to replace this hard-coded transform.
// 	// return Rz(−rz)Ry(−ry)Rx(−rx)T(−c)
// 	return RotateZ(-cam.rotation.z * (M_PI/180.f)) 
// 		* RotateY(-cam.rotation.y * (M_PI/180.f)) 
// 		* RotateX(-cam.rotation.x * (M_PI/180.f)) 
// 		* Translate(-cam.position.x, -cam.position.y, -cam.position.z);
// }
