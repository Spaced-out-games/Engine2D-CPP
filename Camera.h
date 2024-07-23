#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

// TODO: Use vec3s, quaternions for orientation; deprecate matrices
class Camera {
public:
    Camera(const glm::vec3& position = glm::vec3(0.0f), const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = -90.0f, float pitch = 0.0f, float fov = 45.0f, float aspect = 16.0f / 9.0f,
        float near = 0.1f, float far = 100.0f)
    {
        this->position = position;
        this->worldUp = up;
        this->yaw = yaw;
        this->pitch = pitch;
        this->fov = fov;
        this->aspect = aspect;
        this->near = near;
        this->far = far;

        updateCameraVectors();
        updateProjectionMatrix();
        updateViewMatrix();
        updateVprojUniform();
    }

    void setPosition(const glm::vec3& position) {
        this->position = position;
        updateViewMatrix();
        updateVprojUniform();
    }

    void setRotation(float yaw, float pitch) {
        this->yaw = yaw;
        this->pitch = pitch;
        updateCameraVectors();
        updateViewMatrix();
        updateVprojUniform();
    }

    void setProjection(float fov, float aspect, float near, float far) {
        this->fov = fov;
        this->aspect = aspect;
        this->near = near;
        this->far = far;
        updateProjectionMatrix();
        updateVprojUniform();
    }

    glm::vec3 getPosition() const { return position; }
    glm::vec3 getFront() const { return front; }
    glm::vec3 getUp() const { return up; }
    glm::vec3 getRight() const { return right; }
    glm::mat4 getProjectionMatrix() const { return projection; }
    glm::mat4 getViewMatrix() const { return view; }

    void setUniformLocations(GLuint programID) {
        projectionLoc = glGetUniformLocation(programID, "projection");
        viewLoc = glGetUniformLocation(programID, "view");
        vprojLoc = glGetUniformLocation(programID, "vproj");
    }

    void updateUniforms() {
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    }

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    float yaw;
    float pitch;
    float fov;
    float aspect;
    float near;
    float far;
    glm::mat4 projection;
    glm::mat4 view;
    GLuint projectionLoc;
    GLuint viewLoc;
    GLuint vprojLoc;

    void updateCameraVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        this->front = glm::normalize(front);
        this->right = glm::normalize(glm::cross(this->front, this->worldUp));
        this->up = glm::normalize(glm::cross(this->right, this->front));
    }

    void updateViewMatrix() {
        view = glm::lookAt(position, position + front, up);
    }

    void updateProjectionMatrix() {
        projection = glm::perspective(glm::radians(fov), aspect, near, far);
    }

    void updateVprojUniform() {
        updateUniforms();
    }
    void lookAt(const glm::vec3& target) {
        // Set the front vector to the direction from position to target
        front = glm::normalize(target - position);
        // Recalculate right and up vectors
        updateCameraVectors();
        // Update view matrix
        updateViewMatrix();
        // Update Vproj uniform
        updateVprojUniform();
    }
};