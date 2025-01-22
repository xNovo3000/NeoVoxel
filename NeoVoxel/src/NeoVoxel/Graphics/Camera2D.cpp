#include <PCH.h>
#include <NeoVoxel/Graphics/Camera2D.h>

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace NeoVoxel {

	Camera2D::Camera2D() : m_Position(0.0F), m_Roll(0.0F) {}

	void Camera2D::setPosition(const glm::vec2& position) {
		m_Position = position;
	}

	void Camera2D::setRoll(float roll) {
		m_Roll = roll;
		while (m_Roll > glm::pi<float>()) {
			m_Roll -= glm::two_pi<float>();
		}
		while (m_Roll <= -glm::pi<float>()) {
			m_Roll += glm::two_pi<float>();
		}
	}

	glm::mat4 Camera2D::getViewMatrix() const {
		auto result = glm::mat4(1.0F);
		result = glm::rotate(result, -m_Roll, glm::vec3(0.0F, 0.0F, 1.0F));
		result = glm::translate(result, -glm::vec3(m_Position, 0.0F));
		return result;
	}

}