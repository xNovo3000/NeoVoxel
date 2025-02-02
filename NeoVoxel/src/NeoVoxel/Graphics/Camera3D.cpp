#include <PCH.h>
#include <NeoVoxel/Graphics/Camera3D.h>

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace NeoVoxel {

	Camera3D::Camera3D() : m_Position(0.0F), m_Pitch(0.0F), m_Yaw(0.0F), m_Roll(0.0F) {}

	void Camera3D::setPosition(const glm::vec3& position) {
		m_Position = position;
	}

	void Camera3D::setPitch(float pitch) {
		m_Pitch = pitch;
		while (m_Pitch > glm::pi<float>()) {
			m_Pitch -= glm::two_pi<float>();
		}
		while (m_Pitch <= -glm::pi<float>()) {
			m_Pitch += glm::two_pi<float>();
		}
	}

	void Camera3D::setYaw(float yaw) {
		m_Yaw = yaw;
		while (m_Yaw > glm::pi<float>()) {
			m_Yaw -= glm::two_pi<float>();
		}
		while (m_Yaw <= -glm::pi<float>()) {
			m_Yaw += glm::two_pi<float>();
		}
	}

	void Camera3D::setRoll(float roll) {
		m_Roll = roll;
		while (m_Roll > glm::pi<float>()) {
			m_Roll -= glm::two_pi<float>();
		}
		while (m_Roll <= -glm::pi<float>()) {
			m_Roll += glm::two_pi<float>();
		}
	}

	glm::mat4 Camera3D::getViewMatrix() const {
		auto result = glm::mat4(1.0F);
		result = glm::rotate(result, -m_Roll, glm::vec3(0.0F, 0.0F, 1.0F));
		result = glm::rotate(result, -m_Yaw, glm::vec3(0.0F, 1.0F, 0.0F));
		result = glm::rotate(result, -m_Pitch, glm::vec3(1.0F, 0.0F, 0.0F));
		result = glm::translate(result, -m_Position);
		return result;
	}

}