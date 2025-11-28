#include "Camera.h"
#include "Player.h"
#include <algorithm>
#include <cmath>

Camera::Camera(Player* player)
    : m_player(player)
    , m_mode(CameraMode::FIRST_PERSON)
    , m_position(0.0f, 1.6f, 0.0f)
    , m_front(0.0f, 0.0f, -1.0f)
    , m_up(0.0f, 1.0f, 0.0f)
    , m_worldUp(0.0f, 1.0f, 0.0f)
    , m_yaw(-90.0f)
    , m_pitch(0.0f)
    , m_fov(45.0f)
    , m_aspect(16.0f / 9.0f)
    , m_near(0.1f)
    , m_far(100.0f)
    , m_distance(5.0f)
    , m_height(2.0f)
    , m_targetPosition(0.0f)
{
    UpdateCameraVectors();
}

void Camera::Update(float deltaTime) {
    if (m_mode == CameraMode::FIRST_PERSON) {
        UpdateFirstPerson();
    } else {
        UpdateThirdPerson(deltaTime);
    }
}

void Camera::UpdateFirstPerson() {
    if (!m_player) return;
    
    glm::vec3 playerPos = m_player->GetPosition();
    m_position = playerPos + glm::vec3(0.0f, 1.6f, 0.0f); // Eye level
    UpdateCameraVectors();
}

void Camera::UpdateThirdPerson(float deltaTime) {
    if (!m_player) return;
    
    glm::vec3 playerPos = m_player->GetPosition();
    
    // Calculate desired camera position behind player
    glm::vec3 desiredPos = playerPos - m_front * m_distance + glm::vec3(0.0f, m_height, 0.0f);
    
    // Smooth interpolation
    float lerpSpeed = 5.0f;
    m_targetPosition = glm::mix(m_targetPosition, desiredPos, lerpSpeed * deltaTime);
    m_position = m_targetPosition;
    
    UpdateCameraVectors();
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset) {
    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    
    m_yaw += xoffset;
    m_pitch += yoffset;
    
    // Constrain pitch
    m_pitch = std::clamp(m_pitch, -89.0f, 89.0f);
    
    UpdateCameraVectors();
}

void Camera::ToggleMode() {
    m_mode = (m_mode == CameraMode::FIRST_PERSON) ? CameraMode::THIRD_PERSON : CameraMode::FIRST_PERSON;
    if (m_mode == CameraMode::THIRD_PERSON) {
        m_targetPosition = m_position;
    }
}

void Camera::Reset() {
    m_yaw = -90.0f;
    m_pitch = 0.0f;
    UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const {
    if (m_mode == CameraMode::FIRST_PERSON) {
        return glm::lookAt(m_position, m_position + m_front, m_up);
    } else {
        // Third person: look at player
        if (m_player) {
            glm::vec3 target = m_player->GetPosition() + glm::vec3(0.0f, 1.0f, 0.0f);
            return glm::lookAt(m_position, target, m_up);
        }
        return glm::lookAt(m_position, m_position + m_front, m_up);
    }
}

glm::mat4 Camera::GetProjectionMatrix() const {
    float fovRad = m_fov * 3.14159265359f / 180.0f;
    return glm::perspective(fovRad, m_aspect, m_near, m_far);
}

void Camera::UpdateCameraVectors() {
    float yawRad = m_yaw * 3.14159265359f / 180.0f;
    float pitchRad = m_pitch * 3.14159265359f / 180.0f;
    
    glm::vec3 front;
    front.x = std::cos(yawRad) * std::cos(pitchRad);
    front.y = std::sin(pitchRad);
    front.z = std::sin(yawRad) * std::cos(pitchRad);
    m_front = glm::normalize(front);
    
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}

