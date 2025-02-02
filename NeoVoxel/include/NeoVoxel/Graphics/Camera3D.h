#pragma once

#include <NeoVoxel/Core/Base.h>

#include <glm/glm.hpp>

namespace NeoVoxel {

	class Camera3D {

	public:
		Camera3D();
		~Camera3D() = default;

		Camera3D(const Camera3D&) = default;
		Camera3D& operator=(const Camera3D&) = default;

		Camera3D(Camera3D&&) noexcept = default;
		Camera3D& operator=(Camera3D&&) noexcept = default;

		void setPosition(const glm::vec3& position);
		void setPitch(float pitch);
		void setYaw(float yaw);
		void setRoll(float roll);

		const glm::vec3& getPosition() const noexcept { return m_Position; }
		float getPitch() const noexcept { return m_Pitch; }
		float getYaw() const noexcept { return m_Yaw; }
		float getRoll() const noexcept { return m_Roll; }

		glm::mat4 getViewMatrix() const;

	private:
		glm::vec3 m_Position;
		float m_Pitch, m_Yaw, m_Roll;

	};

}