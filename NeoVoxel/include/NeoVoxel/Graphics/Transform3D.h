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
		void setRotation(const glm::vec3& rotation);
		void setScale(float scale);

		const glm::vec3& getPosition() const noexcept { return m_Position; }
		const glm::vec3& getRotation() const noexcept { return m_Rotation; }
		float getScale() const noexcept { return m_Scale; }

		glm::mat4 getModelMatrix() const;

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
		float m_Scale;

	};

}