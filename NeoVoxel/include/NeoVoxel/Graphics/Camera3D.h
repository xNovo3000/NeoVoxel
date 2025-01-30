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
		void setRotation(const glm::vec3& rotation);

		const glm::vec3& getPosition() const noexcept { return m_Position; }
		const glm::vec3& getRotation() const noexcept { return m_Rotation; }

		glm::mat4 getViewMatrix() const;

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Rotation;

	};

}