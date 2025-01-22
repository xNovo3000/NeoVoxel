#include <PCH.h>
#include <NeoVoxel/Graphics/Transform2D.h>

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

	glm::mat4 Transform2D::getModelMatrix() const {
		auto result = glm::mat4(1.0F);
		result = glm::scale(result, glm::vec3(m_Scale));
		result = glm::rotate(result, -m_Roll, glm::vec3(0.0F, 0.0F, 1.0F));
		result = glm::translate(result, -glm::vec3(m_Position, 0.0F));
		return result;
	}

}