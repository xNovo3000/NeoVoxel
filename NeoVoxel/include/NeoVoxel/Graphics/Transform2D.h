#pragma once

#include <NeoVoxel/Core/Base.h>

#include <glm/glm.hpp>

namespace NeoVoxel {

	class Transform2D {

	public:
		Transform2D();
		~Transform2D() = default;

		Transform2D(const Transform2D&) = default;
		Transform2D& operator=(const Transform2D&) = default;

		Transform2D(Transform2D&&) noexcept = default;
		Transform2D& operator=(Transform2D&&) noexcept = default;

		void setPosition(const glm::vec2& position);
		void setRoll(float roll);
		void setScale(float scale);

		const glm::vec2& getPosition() const noexcept { return m_Position; }
		float getRoll() const noexcept { return m_Roll; }
		float getScale() const noexcept { return m_Scale; }

		glm::mat4 getViewMatrix() const;

	private:
		glm::vec2 m_Position;
		float m_Roll;
		float m_Scale;

	};

}