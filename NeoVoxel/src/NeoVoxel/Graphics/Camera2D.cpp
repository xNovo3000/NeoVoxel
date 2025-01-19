#include <PCH.h>
#include <NeoVoxel/Graphics/Camera2D.h>

#include <glm/gtc/constants.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

namespace NeoVoxel {

	Camera2D::Camera2D() : Camera2D({ 0.0F, 0.0F }, 0.0F) {}
	Camera2D::Camera2D(const glm::vec2& position) : Camera2D(position, 0.0F) {}
	Camera2D::Camera2D(const glm::vec2& position, float roll) : m_Position(position), m_Roll(roll) {}

	void Camera2D::setPosition(const glm::vec2& position) {
		m_Position = position;
	}

	void Camera2D::setRoll(float roll) {
		m_Roll += roll;
		while (m_Roll > glm::pi<float>()) {
			m_Roll -= glm::two_pi<float>();
		}
		while (m_Roll <= -glm::pi<float>()) {
			m_Roll += glm::two_pi<float>();
		}
	}

	glm::mat3 Camera2D::getViewMatrix() const {
		auto result = glm::mat3(1.0F);
		result = glm::rotate(result, -m_Roll);
		result = glm::translate(result, -m_Position);
		return result;
	}

}