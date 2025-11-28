#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Player;

enum class CameraMode {
    FIRST_PERSON,
    THIRD_PERSON
};

class Camera {
public:
    Camera(Player* player);
    
    void Update(float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset);
    void ToggleMode();
    void Reset();
    
    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix() const;
    
    glm::vec3 GetPosition() const { return m_position; }
    glm::vec3 GetForward() const { return m_front; }
    glm::vec3 GetRight() const { return m_right; }
    glm::vec3 GetUp() const { return m_up; }
    
    void SetAspectRatio(float aspect) { m_aspect = aspect; }
    
    CameraMode GetMode() const { return m_mode; }

private:
    Player* m_player;
    CameraMode m_mode;
    
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp;
    
    float m_yaw;
    float m_pitch;
    float m_fov;
    float m_aspect;
    float m_near;
    float m_far;
    
    // Third-person camera settings
    float m_distance;
    float m_height;
    glm::vec3 m_targetPosition;
    
    void UpdateCameraVectors();
    void UpdateFirstPerson();
    void UpdateThirdPerson(float deltaTime);
};

#endif // CAMERA_H

