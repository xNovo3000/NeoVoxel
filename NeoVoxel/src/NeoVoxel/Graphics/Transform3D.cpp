#include <PCH.h>
#include <NeoVoxel/Graphics/Transform3D.h>

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace NeoVoxel {

	Transform3D::Transform3D() : m_Position(0.0F), m_Pitch(0.0F), m_Yaw(0.0F), m_Roll(0.0F), m_Scale(1.0F) {}

	void Transform3D::setPosition(const glm::vec3& position) {
		m_Position = position;
	}

	void Transform3D::setPitch(float pitch) {
		m_Pitch = pitch;
		while (m_Pitch > glm::pi<float>()) {
			m_Pitch -= glm::two_pi<float>();
		}
		while (m_Pitch <= -glm::pi<float>()) {
			m_Pitch += glm::two_pi<float>();
		}
	}

	void Transform3D::setYaw(float yaw) {
		m_Yaw = yaw;
		while (m_Yaw > glm::pi<float>()) {
			m_Yaw -= glm::two_pi<float>();
		}
		while (m_Yaw <= -glm::pi<float>()) {
			m_Yaw += glm::two_pi<float>();
		}
	}

	void Transform3D::setRoll(float roll) {
		m_Roll = roll;
		while (m_Roll > glm::pi<float>()) {
			m_Roll -= glm::two_pi<float>();
		}
		while (m_Roll <= -glm::pi<float>()) {
			m_Roll += glm::two_pi<float>();
		}
	}

	void Transform3D::setScale(float scale) {
		m_Scale = std::clamp(scale, 0.001F, 1000.0F);
	}

	glm::mat4 Transform3D::getModelMatrix() const {
		auto result = glm::mat4(1.0F);
		result = glm::scale(result, glm::vec3(m_Scale));
		result = glm::rotate(result, m_Roll, glm::vec3(0.0F, 0.0F, 1.0F));
		result = glm::rotate(result, m_Yaw, glm::vec3(0.0F, 1.0F, 0.0F));
		result = glm::rotate(result, m_Pitch, glm::vec3(1.0F, 0.0F, 0.0F));
		result = glm::translate(result, m_Position);
		return result;
	}

}