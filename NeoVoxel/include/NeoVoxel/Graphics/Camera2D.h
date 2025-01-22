#pragma once

#include <NeoVoxel/Core/Base.h>

#include <glm/glm.hpp>

namespace NeoVoxel {

	class Camera2D {

	public:
		Camera2D();
		~Camera2D() = default;

		Camera2D(const Camera2D&) = default;
		Camera2D& operator=(const Camera2D&) = default;

		Camera2D(Camera2D&&) noexcept = default;
		Camera2D& operator=(Camera2D&&) noexcept = default;

		void setPosition(const glm::vec2& position);
		void setRoll(float roll);

		const glm::vec2& getPosition() const noexcept { return m_Position; }
		float getRoll() const noexcept { return m_Roll; }

		glm::mat4 getViewMatrix() const;

	private:
		glm::vec2 m_Position;
		float m_Roll;

	};

}