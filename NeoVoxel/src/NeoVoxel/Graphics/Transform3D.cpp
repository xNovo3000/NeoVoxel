#include <PCH.h>
#include <NeoVoxel/Graphics/Transform3D.h>

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace NeoVoxel {

	Transform3D::Transform3D() : m_Position(0.0F), m_Rotation(0.0F), m_Scale(1.0F) {}

	void Transform3D::setPosition(const glm::vec3& position) {
		m_Position = position;
	}

	void Transform3D::setRotation(const glm::vec3& rotation) {
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

	void Transform3D::setScale(float scale) {
		m_Scale = std::clamp(scale, 0.001F, 1000.0F);
	}

	glm::mat4 Transform3D::getModelMatrix() const {
		auto result = glm::mat4(1.0F);
		result = glm::scale(result, glm::vec3(m_Scale));
		result = glm::rotate(result, m_Rotation.z, glm::vec3(0.0F, 0.0F, 1.0F));
		result = glm::rotate(result, m_Rotation.y, glm::vec3(0.0F, 1.0F, 0.0F));
		result = glm::rotate(result, m_Rotation.x, glm::vec3(1.0F, 0.0F, 0.0F));
		result = glm::translate(result, m_Position);
		return result;
	}

}