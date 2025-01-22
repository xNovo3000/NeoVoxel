#include <PCH.h>
#include <NeoVoxel/Graphics/Transform2D.h>

#include <glm/gtc/constants.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

namespace NeoVoxel {

	Transform2D::Transform2D() : m_Position(0.0F), m_Roll(0.0F), m_Scale(1.0F) {}

	void Transform2D::setPosition(const glm::vec2& position) {
		m_Position = position;
	}

	void Transform2D::setRoll(float roll) {
		m_Roll = roll;
		while (m_Roll > glm::pi<float>()) {
			m_Roll -= glm::two_pi<float>();
		}
		while (m_Roll <= -glm::pi<float>()) {
			m_Roll += glm::two_pi<float>();
		}
	}

	void Transform2D::setScale(float scale) {
		m_Scale = std::clamp(scale, 0.001F, 1000.0F);
	}

	glm::mat3 Transform2D::getViewMatrix() const {
		auto result = glm::mat3(1.0F);
		result = glm::scale(result, glm::vec2(m_Scale));
		result = glm::rotate(result, -m_Roll);
		result = glm::translate(result, -m_Position);
		return result;
	}

}