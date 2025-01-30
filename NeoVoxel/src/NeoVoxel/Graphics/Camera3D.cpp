#include <PCH.h>
#include <NeoVoxel/Graphics/Camera3D.h>

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace NeoVoxel {

	Camera3D::Camera3D() : m_Position(0.0F), m_Rotation(0.0F) {}

	void Camera3D::setPosition(const glm::vec3& position) {
		m_Position = position;
	}

	void Camera3D::setRotation(const glm::vec3& rotation) {
		m_Rotation = rotation;
		while (m_Rotation.x > glm::pi<float>()) {
			m_Rotation.x -= glm::two_pi<float>();
		}
		while (m_Rotation.x <= -glm::pi<float>()) {
			m_Rotation.x += glm::two_pi<float>();
		}
		while (m_Rotation.y > glm::pi<float>()) {
			m_Rotation.y -= glm::two_pi<float>();
		}
		while (m_Rotation.y <= -glm::pi<float>()) {
			m_Rotation.y += glm::two_pi<float>();
		}
		while (m_Rotation.z > glm::pi<float>()) {
			m_Rotation.z -= glm::two_pi<float>();
		}
		while (m_Rotation.z <= -glm::pi<float>()) {
			m_Rotation.z += glm::two_pi<float>();
		}
	}

	glm::mat4 Camera3D::getViewMatrix() const {
		auto result = glm::mat4(1.0F);
		result = glm::rotate(result, -m_Rotation.z, glm::vec3(0.0F, 0.0F, 1.0F));
		result = glm::rotate(result, -m_Rotation.y, glm::vec3(0.0F, 1.0F, 0.0F));
		result = glm::rotate(result, -m_Rotation.x, glm::vec3(1.0F, 0.0F, 0.0F));
		result = glm::translate(result, -m_Position);
		return result;
	}

}