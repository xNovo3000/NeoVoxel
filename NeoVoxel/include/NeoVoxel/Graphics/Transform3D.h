#pragma once

#include <NeoVoxel/Core/Base.h>

#include <glm/glm.hpp>

namespace NeoVoxel {

	class Transform3D {

	public:
		Transform3D();
		~Transform3D() = default;

		Transform3D(const Transform3D&) = default;
		Transform3D& operator=(const Transform3D&) = default;

		Transform3D(Transform3D&&) noexcept = default;
		Transform3D& operator=(Transform3D&&) noexcept = default;

		void setPosition(const glm::vec3& position);
		void setPitch(float pitch);
		void setYaw(float yaw);
		void setRoll(float roll);
		void setScale(float scale);

		const glm::vec3& getPosition() const noexcept { return m_Position; }
		float getPitch() const noexcept { return m_Pitch; }
		float getYaw() const noexcept { return m_Yaw; }
		float getRoll() const noexcept { return m_Roll; }
		float getScale() const noexcept { return m_Scale; }

		glm::mat4 getModelMatrix() const;

	private:
		glm::vec3 m_Position;
		float m_Pitch, m_Yaw, m_Roll;
		float m_Scale;

	};

}